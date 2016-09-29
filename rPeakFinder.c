#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/rPeakFinder.h"
#include "includes/peakSearcher.h"
#include "includes/avgCircularArray.h"
#include "includes/peakAvgCircularArray.h"


#define MILISECONDS_PER_MINUTE (1000 * 60)
#define SIZE_ALL_PEAKS_ARRAY 16
#define AVERAGE_NUMBER_MEMBERS 8 //Number of members in the average arrays, meaning RecentRR and RecentRR_OK
#define MISSES_FOR_UNSTABLE 5 //Number of time a peak can be missed before the pulse is unstable (*)
#define DEFAULT_RPEAK_RR_VALUE 150 //The default RR value for a R-peak.
#define DEFAULT_AVERAGE_RPEAK_INTENSITY 4500 //The average intensity of a true R-peak.
#define DEFAULT_AVERAGE_NOICEPEAK_INTENSITY 1000 //The average value of a noice peak.

//Average circular array for the last 8 true R-peaks found and recorded
static AvgCircularArray RecentRR;
//Average circular array for the last 8 true R-peaks found and recorded, with an RR value between RR_LOW and RR_HIGH
static AvgCircularArray RecentRR_OK;
static PeakAvgCircularArray trueRPeaks; //Last eight true r peak, also containing the average sum of the RR values
static Peak allPeaks[SIZE_ALL_PEAKS_ARRAY];//Last peaks found since the last R-peak
static int indexAllPeaks = 0; //Index of allPeaks, pointing to the next free space.

/*Variables for detecting R peaks:*/
//Represent the average intensity of a true R-peak.
//Looking at the data (see initializeRPeakFinder), this is approximately the value found on average.
static unsigned short Spkf = DEFAULT_AVERAGE_RPEAK_INTENSITY;
//Represents the average value of a noice peak. Looking at the data, this is approximately the value found, though it vary's quite a bit.
static unsigned short Npkf = DEFAULT_AVERAGE_NOICEPEAK_INTENSITY;
//Peaks need to have a higher intensity than Threshold1, to get checked if it is an R-peak, or trigger a searchback.
static unsigned short Threshold1 =  1875; /*Answers to: Npkf + (Spkf-Npkf)/4*/
//Peaks found through searchback must have a value above Threshold2 to be able to be recorded as a true R-peak.
static unsigned short Threshold2 = 938; /*Answers to Threshold1/2, rounded up*/


/*Variables for finding the RR-interval
 * Given as though calculated from RR_Average2
  */

/*Average of all the peaks in RecentRR. Value explanation in initializeRPeakFinder.
 *It is an int instead of a short, so that when it is used to calculate RR_Low, RR_High and RR_Miss,
 *no overflow will happen; see code for that.
 */
static unsigned int RR_AVERAGE1 = DEFAULT_RPEAK_RR_VALUE;
/*Average of all the peaks in RecentRR_OK. Value explanation in initializeRPeakFinder.
 *It is an int instead of a short, so that when it is used to calculate RR_Low, RR_High and RR_Miss,
 *no overflow will happen; see code for that.
 */
static unsigned int RR_AVERAGE2 = DEFAULT_RPEAK_RR_VALUE;
//Estimate of how much the RR value of a peak must at least be, for it to be a true R peak. This is unless it is in a searchback.
//0.92 times the current average used, rounded down, starting with RR_AVERAGE2 and thus 0.92*150=138
static unsigned short RR_Low = 138;
//Estimate of how much the RR value of a peak must at most be, for it to be a true R peak. This is unless it is in a searchback.
//1.16 times the current average used, rounded down, starting with RR_AVERAGE2 and thus 1.16*150=173
static unsigned short RR_High = 173;
//Estimate of how great the RR value of a peak needs to be, for there to be a great likelihood that a peak has been missed.
//If a peak has an intensity above Treshold1 and an RR value above RR_MISS a searchback i triggered.
//1.66 times the current average used, rounded down, starting with RR_AVERAGE2 and thus 1.66*150=249
static unsigned short RR_Miss = 249;

//Number of times the check for a peak's RR value being between RR_LOW and RR_HIGH has been missed in a row.
static unsigned short concurrentMissedRRLOWAndHigh = 0;
//Number of new true R peaks found since the last time checked with getNewRPeaksFoundCount().
static unsigned char numberNewRPeaksFound = 0;

/* Initializes the rPeakFinder, by setting inserting in to it's different circular and average circular arrays,
 * some initial values. Must be run before isRPeak.
 *  */
void initializeRPeakFinder(){
	//Initializes the arrays with the given default value (the first two), start index and size;
	/* Looking from the data, the intensity of a normal heart beat is around 4500.
	 * Looking at the Internet, see http://www.heart.org/HEARTORG/HealthyLiving/PhysicalActivity/Target-Heart-Rates_UCM_434341_Article.jsp
	 * the average resting heat rate, the average number of heartbeats per minut, is around 60-100 bpm.
	 * Taking 100 as the average, with 250 measurements in a second, it corresponds to about 150 measurements per heartbeat.
	 * This also fits the data well.
	 */
	initAvgCircArray(&RecentRR_OK, AVERAGE_NUMBER_MEMBERS, 0, DEFAULT_RPEAK_RR_VALUE);
	/* About the same as above.
	 * */
	initAvgCircArray(&RecentRR, AVERAGE_NUMBER_MEMBERS, 0, DEFAULT_RPEAK_RR_VALUE);
	Peak defaultPeak = {.RR = DEFAULT_RPEAK_RR_VALUE,.intensity = DEFAULT_AVERAGE_RPEAK_INTENSITY};
	initPeakAvgCircArray(&trueRPeaks, AVERAGE_NUMBER_MEMBERS,0, defaultPeak);
}

/* Moves numberToBeMoved elements after indexNewStart in allPeaks, to the beginning of the array.
 * It also subtracts RRValueUpdate from peaks RR values*/
void movePeaksBackwardsWithRRUpdate(short indexNewStart, short numberToBeMoved, unsigned short RRValueUpdate){
	for(int i = 0; i < numberToBeMoved; i++){
		allPeaks[i] = allPeaks[i + indexNewStart];
		allPeaks[i].RR -= RRValueUpdate;
	}
}

/* Given a new Peak to be recorded as a new true R peak, it records it as such.
   *
 * Peak newPeak; The  peak to be recorded.
 * */
static void recordNewProperRPeak(Peak newPeak){
	//The peak is registrated as a true RR peak.
	insertPeakAvgCircData(&trueRPeaks, newPeak);
	//Calculates the new values for determining if a peak is an RR peak:
	Spkf = (7*Spkf + newPeak.intensity)/8; //Faster and more precise than (7*Spkf)/8 +  newPeak.intensity/8
	//Since it is a true R peak with an RR value between RR_LOW and RR_HIGH, it is recorded in both RecentRR_OK and RecentRR.
	insertAvgCircData(&RecentRR_OK, newPeak.RR);
	insertAvgCircData(&RecentRR, newPeak.RR);
	RR_AVERAGE2 = getAvgCircAverageValue(&RecentRR_OK);
	RR_AVERAGE1 = getAvgCircAverageValue(&RecentRR);
	RR_Low = (23 * RR_AVERAGE2) / 25; /*23/25= 0.92*/
	RR_High = (29 * RR_AVERAGE2) / 25; /*29/25= 1.16*/
	RR_Miss = (5 * RR_AVERAGE2) / 3; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf - Npkf) / 4;
	Threshold2 = Threshold1 / 2;
	numberNewRPeaksFound++;
	//Sets it so that the RR value of the later peaks, is measured according to this R-peak.

}

/*Checks if a given peak to be checked during searchback has an intensity over THRESHOLD2,
 *and if so, it records it as a peak found by searchback.
  *
 *Peak peakToCheck; The Peak, whichs intensity is to be checked.
  *
 *It returns 1 if the peak given has an intensity above THRESHOLD2, else 0*/
static char checkSearchback(Peak peakToCheck){
	if(peakToCheck.intensity <= Threshold2) {
		return 0;
	}
	/*Recording it as an proper R-peak. Will always be later than or the same as the current RPeak*/
	insertPeakAvgCircData(&trueRPeaks, peakToCheck);
	//Calculates the new values for determining if a peak is an RR peak:
	Spkf = (3 * Spkf + peakToCheck.intensity) / 4;
	insertAvgCircData(&RecentRR, peakToCheck.RR);
	RR_AVERAGE1 = getAvgCircAverageValue(&RecentRR);
	RR_Low = 23 * RR_AVERAGE1 / 25; /*23/25= 0.92*/
	RR_High = 29 * RR_AVERAGE1 / 25; /*29/25= 1.16*/
	RR_Miss = 83 * RR_AVERAGE1 / 50; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf - Npkf) / 4;
	Threshold2 = Threshold1 / 2;
	numberNewRPeaksFound++;
	concurrentMissedRRLOWAndHigh=0;
	return 1;
}

/* Finds the index of the most suitable peak to be recorded as a true R peak in allPeaks,
 * from the start, up to the given index, indexMiss. Used during searchbacks.
 *
 * int indexMiss; the index of the last peak in allPeaks, to take into account for the calculations.
 *
 * returns the index to the first suitable peak found (that is recorded by the method checkSearchback), else -1
 *
 * */
static char searchBackBackwardsGoer(int indexMiss){
	for(int i = indexMiss - 1; i >= 0; i--){
		if(checkSearchback(allPeaks[i])){
			return i;
		}
	}
	//If there a no proper peak:
	return -1;
}

/*Checks if a given intensity of a peak, has an intensity greater than THRESHOLD1.
 * If not, it updates NPKF., THRESHOLD1 and THRESHOLD2.
 *
 * Returns 1 if it is above THRESHOLD1, else 0.
 * */
static char passThreshold1(unsigned short intensity){
	if (!(intensity > Threshold1)) { /*If it isn't an R-Peak..*/
			Npkf = (intensity + 7*Npkf)/8; // More precise (and faster) than NPF = intensity/8 + 7*Npkf/8
			Threshold1 = Npkf + (Spkf-Npkf)/4;
			Threshold2 = Threshold1/2;
			return 0;
	} else return 1;
}

/* Given an index indexNewRPeak corresponding to a newly registrated R-peak in allPeaks,
 * it updates indexAllPeaks so that when all the elements after the elements
 * after indexNewRPeak are moved to the beginning, it is correct.
 * It the moves the elements back and updates their values*/
static void searchbackArrayUpdate(int indexNewRPeak){
	//Updates indexAllPeaks:
	indexAllPeaks -= (indexNewRPeak+1);
	/*Moves the elements back, so that all the elements in the array (used currently),
	 *gets moved back so that the element after the one at indexNewRPeak is at position 0.
	 *It also updates their RR value.
	 *Since the peak at the given index is now the newest R-peak,
	 *the later peaks RR value needs to be updated according to this one:
	 */
	movePeaksBackwardsWithRRUpdate(indexNewRPeak+1,indexAllPeaks,allPeaks[indexNewRPeak].RR);
}

/* Activates the searchback procedure ((*)TODO write comment)*/
static char searchBack(){
	char hasFoundNewPeak = 0;
	for(int i = indexAllPeaks-1; i < indexAllPeaks; i++){
		if(passThreshold1(allPeaks[i].intensity)){
			if (RR_Low < allPeaks[i].RR && allPeaks[i].RR < RR_High){
				concurrentMissedRRLOWAndHigh = 0;
				recordNewProperRPeak(allPeaks[i]);
				searchbackArrayUpdate(i);
				i = -1;
			} else {
				concurrentMissedRRLOWAndHigh++;
				if(allPeaks[i].RR > RR_Miss)	{
					//If a peak is above RR_MISS, a new searchback is run from the current peak.
					//Since the peaks all have been placed at the start of allPeaks, the argument for searchBackBackwardsGoer needs to be i-1
					int indexMostBackwards =  searchBackBackwardsGoer(i);
					if(indexMostBackwards != -1){ //If a new candidate could be found, and is recorded:
						//Since, the first time the searchback is run, if there is found any R-peaks at all, it goes here,
						//this is the only place where one needs to set the value;
						hasFoundNewPeak = 1;
						searchbackArrayUpdate(indexMostBackwards);
						i = -1;
					} //Else simply continue from there.
				}
			}
		}
	}
	//Sets the RR value of the later peaks, to be measured from the last found one.
	//Sets it to the latest peak found (allPeaks[indexAllPeaksForSearchback-1])'s RR time
	setTimeSinceLastRPeakFound((indexAllPeaks == 0)? 0 : allPeaks[indexAllPeaks - 1].RR);
	return hasFoundNewPeak;
}

/*Makes the checks to see if a given new Peak should be recorded the normal way as a true R-peak,
 * or if a searchback should be triggered.
 *
 * Peak newPeak; the new Peak to be checked.
 *
 * Returns 1 if a Peak (not necessarily newPeak) gets classified as a true R-Peak, else 0*/
static char rPeakChecks(Peak newPeak){
	if (passThreshold1(newPeak.intensity)){
		/*If it is in the RR-interval*/
		if (RR_Low < newPeak.RR && newPeak.RR < RR_High){
			concurrentMissedRRLOWAndHigh = 0;
			recordNewProperRPeak(newPeak);
			//Sets it so that there have been no new peaks found, since this RR peak.
			indexAllPeaks = 0;
			//Sets the time since the last found R-peak to 0. Can't be done in recordNewProperRPeak,
			//as it is used during searchbacks.
			setTimeSinceLastRPeakFound(0);
			return 1;
		} else {
			//If it should trigger a searchback.
			if(newPeak.RR > RR_Miss)	{
				return searchBack();
			} else	concurrentMissedRRLOWAndHigh++; //Will be incremented inside searchBack() if necessary.
		}
	}
	return 0;
}

/*Moves the last SIZE_ALL_PEAKS_ARRAY/2 in the allPeaks array to the start of the array,
 *and sets indexAllPeaksForSearchback to SIZE_ALL_PEAKS_ARRAY/2.
 *Used when allPeaks is filled up and a new element needs to be inserted*/
static void moveLastPeaksBackInArray(){
	//No rounding errors when dividing will occur since SIZE_ALL_PEAKS_ARRAY is a power of two.
	indexAllPeaks = SIZE_ALL_PEAKS_ARRAY/2;
	memcpy(allPeaks, allPeaks + indexAllPeaks, indexAllPeaks * sizeof(Peak));
}

/* Determines whether a given new peak, is actually an R-peak, or that because of it,
 * that another Peak checked with isRPeak beofre (returning false/0) has been determined to be a true R-peak.
  *
 *Pean newPeak; The Peak to be checked if it is a true R-Peak, or that checking it,
 *										might lead to another peak to be classified as a true R-peak.
 *returns 1 if has found a new RR-peak (it is not neccesarily the one given, if a searchback i performed),
 *							and 0 otherwise.
 * */
char isRPeak(Peak newPeak){
	/* The peak is inserted in the array allPeaks, so that in case of a searchback thorugh the older peaks later,
	 * it can easily be found. In the case that the allPeaks array containing the peaks since the last true R peak is filled up,
	 * the last SIZE_ALL_PEAKS_ARRAY/2 elements are moved back to the beginning, so than an overflow error never will occur.
	 * This will most likely only happen in the case of a heart failure.
	 * */
	if(indexAllPeaks >= SIZE_ALL_PEAKS_ARRAY){
		moveLastPeaksBackInArray();
	}
	allPeaks[indexAllPeaks] = newPeak;
	indexAllPeaks++;
	return rPeakChecks(newPeak);
}

/*Returns the circular array with the true R-Peaks*/
PeakAvgCircularArray* getTrueRPeaksArray(){
	return &trueRPeaks;
}

/*Returns whether the pulse is unstable or not.*/
char isPulseUnstable(){
	return concurrentMissedRRLOWAndHigh >= MISSES_FOR_UNSTABLE;
}

/*Return the number of new R-Peaks found since last time the method was called,
 *and then sets this number to 0.
 * */
char getNewRPeaksFoundCount(){
	char tempStoreNumberNewRPeaksFound = numberNewRPeaksFound;
	numberNewRPeaksFound = 0;
	return tempStoreNumberNewRPeaksFound;
}

/*Resets all the data in RPeakFinder to it's initial values. For testing purposes*/
void resetRPeakFinder(){
	Spkf = DEFAULT_AVERAGE_RPEAK_INTENSITY;
	Npkf = DEFAULT_AVERAGE_NOICEPEAK_INTENSITY;
	Threshold1 = 1875;
	Threshold2 = 938;
	RR_AVERAGE1 = DEFAULT_RPEAK_RR_VALUE;
	RR_AVERAGE2 = DEFAULT_RPEAK_RR_VALUE;
	RR_Low = 138;
	RR_High = 173;
	RR_Miss = 249;
	concurrentMissedRRLOWAndHigh = 0;
	numberNewRPeaksFound = 0;
	freeRPeakFinder();
	initializeRPeakFinder();
	setTimeSinceLastRPeakFound(-2);
	//allPeaks dosen't need to be reseted.
}

/*Gets the pulse of the person, calculated from the average over the latest 8 peaks*/
unsigned short getPulse(){
	//There are 4 milliseconds per mesaurements (1/250=0.004), thus one have the following pulse
	return MILISECONDS_PER_MINUTE / (getPeakAvgCircAverageValue(&trueRPeaks)*4);
}
/*Frees the memory used by the different circular arrays used i rPeakfinder*/
void freeRPeakFinder(){
	freeAvgCirc(&RecentRR);
	freeAvgCirc(&RecentRR_OK);
	freePeakAvgCirc(&trueRPeaks);
}
