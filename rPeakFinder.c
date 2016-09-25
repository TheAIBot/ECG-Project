#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/rPeakFinder.h"
#include "includes/peakSearcher.h"
#include "includes/peakCircularArray.h"
#include "includes/avgCircularArray.h"
#define SIZE_ALL_PEAKS_ARRAY 16
#define AVERAGE_NUMBER_MEMBERS 8 //Number of members in the average arrays, meaning RecentRR and RecentRR_OK
#define MISSES_FOR_UNSTABLE 5 //Number of time a peak can be missed before the pulse is unstable (*)
#define DEFAULT_TRUE_RPEAK_RR_VALUE 150 //TODO describe
#define DEFAULT_AVERAGE_TRUE_RPEAK_RR_VALUE 100

//TODO RR time is probably 1 wrong. Check out later - Jesper
//Average circular array for the last 8 true R-peaks found and recorded
static AvgCircularArray RecentRR;
//Average circular array for the last 8 true R-peaks found and recorded, with an RR value between RR_LOW and RR_HIGH
static AvgCircularArray RecentRR_OK;
static PeakCircularArray trueRPeaks; //Last eight true r peak
static Peak allPeaks[SIZE_ALL_PEAKS_ARRAY];//Last eight peaks found, no matter what.
static int indexAllPeaksForSearchback = 0; //Index of allPeaks, pointing to the next free space.

/*Variables for detecting R peaks:*/
//Represent the average size of a true R-peak.
//Looking at the data (see initializeRPeakFinder), this is approximately the value found on average.
static unsigned short Spkf = 4500;
//Represents the average value of a noice peak. Looking at the data, this is approximately the value found, though it vary's quite a bit.
static unsigned short Npkf = 700;
//Peaks need to have a higher intensity than Threshold1, to get checked if it is an R-peak, or trigger a searchback.
static unsigned short Threshold1 =  1100; /*Answers to: Npkf + (Spkf-Npkf)/4*/
//Peaks found through searchback must have a value above Threshold2 to be able to be recorded as a true R-peak.
static unsigned short Threshold2 = 550; /*Answers to Threshold1/2*/

/*Variables for finding the RR-interval
 * Given as though calculated from RR_Average2
 *TODO TODO check if the averages have the right values below
  */

/*Average of all the peaks in RecentRR. Value explanation in initializeRPeakFinder.
 *It is an int instead of a short, so that when it is used to calculate RR_Low, RR_High and RR_Miss,
 *no overflow will happen; see code for that.
 */
static unsigned int RR_AVERAGE1 = DEFAULT_AVERAGE_TRUE_RPEAK_RR_VALUE;
/*Average of all the peaks in RecentRR_OK. Value explanation in initializeRPeakFinder.
 *It is an int instead of a short, so that when it is used to calculate RR_Low, RR_High and RR_Miss,
 *no overflow will happen; see code for that.
 */
static unsigned int RR_AVERAGE2 = DEFAULT_TRUE_RPEAK_RR_VALUE;
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
//Used for... TODO check this (*)
static unsigned short concurrentMissedRRLOWAndHigh = 0;
//Number of new true R peaks found since the last time checked with getNewRPeaksFoundCount().
static unsigned char numberNewRPeaksFound = 0;

/* Initializes the rPeakFinder, by setting inserting in to it's different circular and average circular arrays,
 * some initial values. Must be run before isRPeak.
 *  */
void initializeRPeakFinder(){

	//Initializes the arrays with the given default value (the first two), start index and size;
	/* Looking from the data, the intensity of a normal heart beat is around 4500.
	 * Sometimes lower, but more often than not, greater. //TODO check(*)
	 * Looking at the Internet, see http://www.heart.org/HEARTORG/HealthyLiving/PhysicalActivity/Target-Heart-Rates_UCM_434341_Article.jsp
	 * the average resting heat rate, the average number of heartbeats per minut, is around 60-100 bpm.
	 * Taking 100 as the average, with 250 measurements in a second, it corresponds to about 150 measurements per heartbeat.
	 * This also fits the data well.
	 */
	initAvgCircArray(&RecentRR_OK, AVERAGE_NUMBER_MEMBERS, 0, DEFAULT_TRUE_RPEAK_RR_VALUE);
	//TODO checks true value, this is wrong.
	/* Lets say that one average there is detected one true R peak per non-R-peak detected, and let the later have an
	 * Average RR value of 50 and an intensity of 700. The the Average RR value is (50 + 150 /2)=100,
	 * and the average intensity is (4500 + 700)/2 = 2600
	 * */
	initAvgCircArray(&RecentRR, AVERAGE_NUMBER_MEMBERS, 0, DEFAULT_AVERAGE_TRUE_RPEAK_RR_VALUE);
	initPeakCircArray(&trueRPeaks, AVERAGE_NUMBER_MEMBERS, 0);
}

/* Given a new Peak to be recorded as a new true R peak, it records it as such.
   *
 * Peak newPeak; The  peak to be recorded.
 * */
static void recordNewProperRPeak(Peak newPeak){
	//Calculates the new values for determining if a peak is an RR peak:
	insertPeakCircArrayData(&trueRPeaks, newPeak);
	Spkf = (7*Spkf + newPeak.intensity)/8; //Faster and more precise than (7*Spkf)/8 +  newPeak.intensity/8
	//Since it is a true R peak with an RR value between RR_LOW and RR_HIGH, it is recorded in both RecentRR_OK and RecentRR.
	insertAvgCircData(&RecentRR_OK, newPeak.RR);
	insertAvgCircData(&RecentRR, newPeak.RR);
	RR_AVERAGE2 = getAvgCircAverageValue(&RecentRR_OK);//TODO change.
	RR_AVERAGE1 = getAvgCircAverageValue(&RecentRR);
	/*TODO remember the update of calculation of the values below*/
	RR_Low = (23 * RR_AVERAGE2) / 25; /*23/25= 0.92*/
	RR_High = (29 * RR_AVERAGE2) / 25; /*29/25= 1.16*/
	RR_Miss = (5 * RR_AVERAGE2) / 3; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf - Npkf) / 4;
	Threshold2 = Threshold1 / 2;
	numberNewRPeaksFound++;
	//The peak is registrated as a true RR peak.
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
	insertPeakCircArrayData(&trueRPeaks, peakToCheck);
	//Calculates the new values for determining if a peak is an RR peak:
	Spkf = (3 * Spkf + peakToCheck.intensity) / 4;
	insertAvgCircData(&RecentRR, peakToCheck.RR);
	RR_AVERAGE1 = getAvgCircAverageValue(&RecentRR);
	//TODO make it so they are all divided witha power of two, because they allows optimization to bitshift when writing assembler
	RR_Low = 23 * RR_AVERAGE1 / 25; /*23/25= 0.92*/
	RR_High = 29 * RR_AVERAGE1 / 25; /*29/25= 1.16*/
	RR_Miss = 83 * RR_AVERAGE1 / 50; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf - Npkf) / 4;
	Threshold2 = Threshold1 / 2;
	numberNewRPeaksFound++;
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

/* Activates the searchback procedure, in the case that a Peak is found with an intensity above THRESHOLD1,
 * and an RR value above RR_MISS. It will (*)*/
static char searchBack(){
	//Finds the index to a Peak among the peaks found since the last true R-Peak was found, not including the one triggering the searchback,
	//where it's intensity is above Treshold2. If there is one, it is registrated as a true R-Peak.
	int indexMostBackwards = searchBackBackwardsGoer(indexAllPeaksForSearchback - 1);
	if(indexMostBackwards == -1){ //If there wheren't any, it stops.
		indexAllPeaksForSearchback = 0;
		return 0;
	} //Else:
	//It then goes through the later Peaks, and checks if they now are true R-peaks.
	//Since a new true R-peak has been found, the later Peak's RR value should be decreased by it's RR value.
	unsigned short newRRRemoval = allPeaks[indexMostBackwards].RR;
	for(int i = indexMostBackwards + 1; i < indexAllPeaksForSearchback; i++){
		allPeaks[i].RR -= newRRRemoval;
		/*Moves the peak back in the array, so that the first peak after the newest found peak is the first in the array:
		 *This can be seen by the fact that (i - indexMostBackwards - 1) = (indexMostBackwards + 1 - indexMostBackwards - 1) = 0 at the start,
		 *when i = indexMostBackwards + 1.
		 */
		allPeaks[i - indexMostBackwards - 1] = allPeaks[i];
		/*Makes the checks for the peak*/
		if(passThreshold1(allPeaks[i].intensity)){
			if (RR_Low < allPeaks[i].RR && allPeaks[i].RR < RR_High){
				concurrentMissedRRLOWAndHigh = 0;
				recordNewProperRPeak(allPeaks[i]);
				//Since this peak now is the newest one, the later peaks RR value needs to be updated according to this one:
				newRRRemoval += allPeaks[i].RR;
				//The index of the last recorded R-peak in the array get's set to the current newly recorded one:
				indexMostBackwards = i;
			} else{
				concurrentMissedRRLOWAndHigh++;
				if(allPeaks[i].RR > RR_Miss)	{
					//If a peak is above RR_MISS (again...) a new searchback is run from the current peak.
					//Since the peaks all have been placed at the start of allPeaks, the argument for searchBackBackwardsGoer needs to be i-1
					int newIndexMostBackwards =  searchBackBackwardsGoer(i - 1);
					if(newIndexMostBackwards != -1){ //If a new candidate could be found, and is recorded:
						//Since this peak now is the newest one, the later peaks RR value needs to be updated according to this one:
						newRRRemoval += allPeaks[i].RR;
						//The index of the last recorded R-peak in the array get's set to the current newly recorded one:
						indexMostBackwards = newIndexMostBackwards;
						//TODO Change i. else it wont continue from the newly found peak.
						//i is set back, so that it will continue from the newly found true R peak.
						//Since the peaks haven't been deleted from there position, those that have already been gone over, will still be there.
						i -= indexMostBackwards;
					} //Else simply continue from there.
				}
			}
		}
	}
	//In the end, indexAllPeaksForSearchback is updated according to the last found peak:
	indexAllPeaksForSearchback = indexAllPeaksForSearchback - indexMostBackwards - 1;
	return 1;
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
			indexAllPeaksForSearchback = 0;
			return 1;
		} else {
			concurrentMissedRRLOWAndHigh++;
			//If it should trigger a searchback.
			if(newPeak.RR > RR_Miss)	{
				return searchBack();
			}
		}
	}
	return 0;
}

/*Moves the last SIZE_ALL_PEAKS_ARRAY/2 in the allPeaks array to the start of the array,
 *and sets indexAllPeaksForSearchback to SIZE_ALL_PEAKS_ARRAY/2.
 *Used when allPeaks is filled up and a new element needs to be inserted*/
static void moveLastPeaksBackInArray(){
	//No rounding errors when dividing will occur since SIZE_ALL_PEAKS_ARRAY is a power of two.
	indexAllPeaksForSearchback = SIZE_ALL_PEAKS_ARRAY/2;
	memcpy(allPeaks, allPeaks + indexAllPeaksForSearchback, indexAllPeaksForSearchback * sizeof(Peak));
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
	if(indexAllPeaksForSearchback >= SIZE_ALL_PEAKS_ARRAY){
		moveLastPeaksBackInArray();
	}
	allPeaks[indexAllPeaksForSearchback] = newPeak;
	indexAllPeaksForSearchback++;
	return rPeakChecks(newPeak);
}

/*Returns the circular array with the true R-Peaks*/
PeakCircularArray* getTrueRPeaksArray(){
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

