#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peak.h"
#include "includes/rPeakFinder.h"
#include "includes/peakSearcher.h"
#include "includes/peakCircularArray.h"
#include "includes/avgCircularArray.h"
#define SIZE_ALL_PEAKS_ARRAY 16
#define AVERAGE_NUMBER_MEMBERS 8
#define MISSES_FOR_UNSTABLE 5
//TODO delete
typedef struct TPeak Peak;
/*TODO Hvad vis sample raten er forskellig?*/

/*TODO
 * The peaks that pass threshold 1 should not be re-recorded. All should be updated.
 * The array that calcultates RR_AVERAGE1 should be updated when a searchback has been run.
 * Should all Peaks be recorded forever? Same with R peaks?
 * Make static? What?
 * Removing Peaks from memory when done with them.
 * */

//TODO RR time is probably 1 wrong. Check out later - Jesper
static AvgCircularArray RecentRR;
static AvgCircularArray RecentRR_OK;
static PeakCircularArray trueRPeaks; //Last eight true r peak
static Peak allPeaks[SIZE_ALL_PEAKS_ARRAY];//Last eight peaks found, no matter what.
static int indexAllPeaksForSearchback = 0; //Index of tempPeaksForSearchback, pointing to the next free space.

/*Variables for detecting R peaks*/
static unsigned short Spkf = 4500;
static unsigned short Npkf = 700;
static unsigned short Threshold1 =  1100; /*Answers to: Npkf + (Spkf-Npkf)/4*/
static unsigned short Threshold2 = 550; /*Answers to Threshold1/2*/

/*Variables for finding the RR-interval
 * Given as though calculated from RR_Average2
 *TODO should it be from RR_Average1 or 2 at the beggining. What are they?
  */
static unsigned short RR_AVERAGE1 = 150;
static unsigned short RR_AVERAGE2 = 150;
static unsigned short RR_Low = 138;  /*TODO, check if there is a possibility of the values becoming "locked"*/
static unsigned short RR_High = 173;
static unsigned short RR_Miss = 249;

static unsigned short concurrentMissedRRLOWAndHigh = 0;
static unsigned char numberNewRPeaksFound = 0;

/* Initializes the rPeakFinder, by setting inserting in to it's different circular and average circular arrays,
 * some initial values. Must be run before isRPeak.
 *  */
void initializeRPeakFinder(){
	//TODO update description.
	/* Looking from the data, the intensity of a normal heart beat is around 4500.
	 * Sometimes lower, but more often than not, greater.
	 * Looking at the Internet, see http://www.heart.org/HEARTORG/HealthyLiving/PhysicalActivity/Target-Heart-Rates_UCM_434341_Article.jsp
	 * the average resting heat rate, the time between an heart beat, and thus also the RR-interval, is around 60-100 bpm.
	 * Taking 100 as the average, with 250 measurements in a second, it corresponds to about 150 measurements per heartbeat.
	 * This also fits the data well.
	 */
	//TODO this needs to be a macro
	const int defaultTrueRPeakRRValue = 150;
	//TODO update description.
	/*Lets say that one average there is detected one true R peak per non-R-peak detected, and let the later have an
	 * Average RR value of 50 and an intensity of 700. The the Average RR value is (50 + 150 /2)=100,
	 * and the average intensity is (4500 + 700)/2 = 2600
	 * */
	//TODO this needs to be a macro
	const int defaultAveragePeakRRValue = 100;
	//Initializes the arrays with the given default value (the first two), start index and size;
	initAvgCircArray(&RecentRR_OK, AVERAGE_NUMBER_MEMBERS, 0, defaultTrueRPeakRRValue);
	initAvgCircArray(&RecentRR, AVERAGE_NUMBER_MEMBERS, 0, defaultAveragePeakRRValue);
	initPeakCircArray(&trueRPeaks, AVERAGE_NUMBER_MEMBERS, 0);
}

/*Given the pointer to a new Peak to be recorded as a new true R peak, it records it as such.
   *
 * Peak* newPeak; Pointer to the new Peak to be recorded.
 * */
void recordNewProperRPeak(Peak newPeak){
	//Calculates the new values for determining if a peak is an RR peak:
	insertPeakCircArrayData(&trueRPeaks, newPeak);
	Spkf = (7*Spkf + newPeak.intensity)/8; //TODO fine with rewrite?
	insertAvgCircData(&RecentRR_OK, newPeak.RR);
	insertAvgCircData(&RecentRR, newPeak.RR);
	RR_AVERAGE2 = getAvgCircAverageValue(&RecentRR_OK);//TODO change.
	RR_AVERAGE1 = getAvgCircAverageValue(&RecentRR);
	/*TODO remember the update of calculation of the values below*/
	RR_Low = (23 * RR_AVERAGE2) / 25; /*23/25= 0.92*/
	RR_High = (29 * RR_AVERAGE2) / 25; /*29/25= 1.16*/
	RR_Miss = (5 * RR_AVERAGE2)/3; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf - Npkf) / 4;
	Threshold2 = Threshold1 / 2;
	numberNewRPeaksFound++;
	/*TODO Discuss Andreas, what if the RR interval becomes large enough that multiplying by 83 makes an overflow error,
	 * For example in the case of a searchback? This is a definite possibility,
	 * */
	//The peak is registrated as a true RR peak.
}

/* Finds the index of the most suitable peak to be recorded as a true R peak in tempPeaksForSearchback,
 * up to the index, indexMiss. Used during searchbacks.
 * Calculates the suitability as the RR distance from the average, taking the peaks that has an RR value greater the RR_HIGH,
 * as being double as close as those below RR_LOW.
 *
 * int indexMiss; the index of the last peak in tempPeaksForSearchback, to take into account for the calculations.
 *
 * returns
 *
 * */
char searchBackBackwardsGoer(int indexMiss){
	for(int i = indexMiss - 1; i >= 0; i--){
		if(checkSearchback(allPeaks[i])){
			return i;
		}
	}
	//If there a no proper peak:
	return -1;
}

char checkSearchback(Peak peakToCheck){
	if(peakToCheck.intensity <= Threshold2) {
		return 0;
	}
	//Calulates the new values for determining if a peak is an RR peak:
	insertPeakCircArrayData(&trueRPeaks, peakToCheck);
	Spkf = (3 * Spkf + peakToCheck.intensity) / 4;
	insertAvgCircData(&RecentRR, peakToCheck.RR);
	RR_AVERAGE1 = getAvgCircAverageValue(&RecentRR);
	//TODO make it so they are all divided witha power of two
	//because they allows optimization to bitshift when writing assembler
	RR_Low = 23 * RR_AVERAGE1 / 25; /*23/25= 0.92*/
	RR_High = 29 * RR_AVERAGE1 / 25; /*29/25= 1.16*/
	RR_Miss = 83 * RR_AVERAGE1 / 50; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf - Npkf) / 4;
	Threshold2 = Threshold1 / 2;
	numberNewRPeaksFound++;
	/*Recording it as an proper R-peak. Will always be later than or the same as the current RPeak*/
	return 1;
}

char searchBack(){
	int indexMostBackwards = searchBackBackwardsGoer(indexAllPeaksForSearchback - 1);
	if(indexMostBackwards == -1){
		indexAllPeaksForSearchback = 0;
		return 0;
	} //Else:
	//For decreasing the value of the RR, since the now last registrated peak;
	unsigned short newRRRemoval = allPeaks[indexMostBackwards].RR;
	for(int i = indexMostBackwards + 1; i < indexAllPeaksForSearchback; i++){
		allPeaks[i].RR -= newRRRemoval;
		//Moves the peak back in the array.
		allPeaks[i - indexMostBackwards - 1] = allPeaks[i];
		/*Makes the checks for the peak*/
		if(passThreshold1(allPeaks[i].intensity)){
			if (RR_Low < allPeaks[i].RR && allPeaks[i].RR < RR_High){
				concurrentMissedRRLOWAndHigh = 0;
				recordNewProperRPeak(allPeaks[i]);
				newRRRemoval += allPeaks[i].RR;
				//The index of the last recorded R-peak in the array get's set:
				indexMostBackwards = i;
			} else{
				concurrentMissedRRLOWAndHigh++;
				if(allPeaks[i].RR > RR_Miss)	{
					int newIndexMostBackwards =  searchBackBackwardsGoer(i - 1);
					if(newIndexMostBackwards != -1){
						indexMostBackwards = newIndexMostBackwards;
						newRRRemoval += allPeaks[i].RR;
					} //Else simply continue.
				}
			}
		}
	}
	indexAllPeaksForSearchback = indexAllPeaksForSearchback - indexMostBackwards - 1;
	return 1;
}

/*Checks if a given Peak peak(pointer), has an intensity greater than THRESHOLD1,
 * */
char passThreshold1(unsigned short intensity){
	if (!(intensity > Threshold1)) { /*If it isn't an R-Peak..*/
			Npkf = (intensity + 7*Npkf)/8; // More precise (and faster) than peak.intensity8 + 7*Npkf/8
			Threshold1 = Npkf + (Spkf-Npkf)/4;
			Threshold2 = Threshold1/2;
			/*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
			return 0;
	} else return 1;
}

char rPeakChecks(Peak newPeak){
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
			if(newPeak.RR > RR_Miss)	{
				return searchBack();
			}
		}
	}
	return 0;
}

/*Determines whether a given new peak, is actually an R-peak.
 *According to this, it also record it,
 *and changes the different variables for calculating if a peak is a true RR peak.
 *
 *int peakValue  = magnitude of the peak filtering.
 *int peakTime_0 = the RR value of the peak, meaning the time since the last recorded RR value.
 *
 *returns 1 if has found a new RR-peak (it is not neccesarily the one given, if a searchback i performed),
 *							and 0 otherwise.
 * */
char isRPeak(Peak newPeak){
	/*In the case that the allPeaks array containing the peaks since the last true R peak is filled up,
	 * the last SIZE_ALL_PEAKS_ARRAY/2 elements are moved back to the beginning,
	 * so than an overflow error never will occur.
	 * This will most likely only happen in the case of a heart failure.
	 * */
	if(indexAllPeaksForSearchback >= SIZE_ALL_PEAKS_ARRAY){
		moveLastPeaksBackInArray();
	}
	allPeaks[indexAllPeaksForSearchback] = newPeak;
	indexAllPeaksForSearchback++;
	return rPeakChecks(newPeak);
}

PeakCircularArray* getTrueRPeaksArray(){
	return &trueRPeaks;
}

void moveLastPeaksBackInArray(){
	//No rounding errors when dividing will occur since SIZE_ALL_PEAKS_ARRAY is a power of two.
	indexAllPeaksForSearchback = SIZE_ALL_PEAKS_ARRAY/2;
	//TODO talk if this is correct
	memcpy(allPeaks, allPeaks + indexAllPeaksForSearchback, indexAllPeaksForSearchback * sizeof(Peak));
	//for(int i = 0; i < indexAllPeaksForSearchback; i++){
	//	allPeaks[i] = allPeaks[i+indexAllPeaksForSearchback];
	//}
}

char isPulseUnstable(){
	return concurrentMissedRRLOWAndHigh >= 5;
}

char getNewRPeaksFoundCount(){
	char tempStoreNumberNewRPeaksFound = numberNewRPeaksFound;
	numberNewRPeaksFound = 0;
	return tempStoreNumberNewRPeaksFound;
}

