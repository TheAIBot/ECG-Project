#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/peakCircularArray.h"
#include "includes/avgCircularArray.h"
#define SIZE_R_ARRAYS 128
#define AVERAGE_NUMBER_MEMBERS 8
#define RR_AVERAGE2 getAvgCircAverageRR(threshold1PassPeaks)

//TODO delete
typedef struct TPeak Peak;

/*Storing*/
/*TODO Explain why the different values*/

/*TODO
 * The peaks that pass threshold 1 should not be re-recorded. All should be updated.
 * The array that calcultates RR_AVERAGE1 should be updated when a searchback has been run.
 * Should all Peaks be recorded forever? Same with R peaks?
 * Make static? What?
 * Removing Peaks from memory when done with them.
 * */

PeakCircularArray trueRRPeaks;
AvgCircularArray threshold1PassPeaks;
struct TAvgCircularArray allPeaksInclusiveOfDoom;//Last eight peaks found, no matter what.
PeakCircularArray allPeaksOverThreshold2; //For searchback.

//struct CircularArray threshold1PassPeaks = {.size = SIZE_R_ARRAYS, .startIndex = 0,		(int[SIZE_R_ARRAYS]) {[0 ... AVERAGE_NUMBER_MEMBERS-1]=150}};
//struct CircularArray threshold1PassVal = {.size = SIZE_R_ARRAYS, .startIndex = 0,		(int[SIZE_R_ARRAYS]) {[0 ... AVERAGE_NUMBER_MEMBERS-1]=3500}};
/* Suppose half the peaks found are noicepeaks with an average RR of 50, the other half are R-peaks,
 *,then the average of that is (50 + 150) / 2 = 100
   */

//struct AvgCircularArray allPeaks = {.size = AVERAGE_NUMBER_MEMBERS, .startIndex = 0,		(int[AVERAGE_NUMBER_MEMBERS]) {[0 ... 7]=100}}; /*TODO delete it. Only for testing purposes, else the same data is stored twice.*/
/* Suppose half the peaks found are noicepeaks with an average value of 300, the other half are R-peaks,
 *,then the average of that is (300 + 3500) / 2 = 1900
   */
//struct CircularArray allPeaksVal = {.size = AVERAGE_NUMBER_MEMBERS, .startIndex = 0, 		(int[AVERAGE_NUMBER_MEMBERS]) {[0 ... 7]=1900}};/*TODO delete it. Only for testing purposes, else the same data is stored twice.*/
/*Indexes for the storing*/

int allPeaksVal[AVERAGE_NUMBER_MEMBERS];
int allPeaksRR[AVERAGE_NUMBER_MEMBERS];

//TODO look at below after structify:
int indexAllPeaks = AVERAGE_NUMBER_MEMBERS; /*TODO delete it. Only for testing purposes*/

/*Variables for detecting R peaks*/
int Spkf = 3500;
int Npkf = 300;
//TODO Might want to make to macros. Depends on performance.
int Threshold1 =  1100; /*Answers to: Npkf + (Spkf-Npkf)/4*/
int Threshold2 = 550; /*Answers to Threshold1/2*/

/*Variables for finding the RR-interval.*/
/*Given the values in the arrays above, one gets:*/
int RR_Average1_Sum = AVERAGE_NUMBER_MEMBERS*100;
int RR_Average1 = 100;
/*Given as though calculated from RR_Average1*/
//TODO What about macros? But problems.
int RR_Low = 138;  /*TODO, check if there is a possibility of the values becoming "locked"*/
int RR_High = 173;
int RR_Miss = 249;

/*Variables used for the searchback*/

Peak tempPeaksForSearchback[16];
int tempIndexPeaksForSearchback = 0;

void initializeRPeakFinder(){
	//Most of the values have been set already, the only thing missing is setting the value of the arrays:
	//TODO Discuss with Andreas.
	Peak trueRRPeaksArray[AVERAGE_NUMBER_MEMBERS];
	for(int i = 0; i<AVERAGE_NUMBER_MEMBERS; i++){
		trueRRPeaksArray[i] = ((struct TPeak){.RR=150, .intensity=3500});
	}
	//initPeakCircArray(&trueRRPeaksOfDoom, AVERAGE_NUMBER_MEMBERS, 0);
	trueRRPeaks.size = AVERAGE_NUMBER_MEMBERS;
	trueRRPeaks.startIndex = 0;
	trueRRPeaks.data = trueRRPeaksArray;
	trueRRPeaks.data[3].RR = 42;
	for(int i = 0; i<AVERAGE_NUMBER_MEMBERS; i++){
		Peak peakdata = 	trueRRPeaks.data[i];
		printf("\n");
	}
	printf("correct?");
}

//TODO check this method
int* getNewestTrueRRPeakTimes(int timesCount)
{
	int* timesSums = malloc(timesCount * sizeof(int));
	int timeSum = 0;
	for(int i = 8; i < timesCount + 8; i++)
	{
		timeSum += trueRRPeakRR[i];
		timesSums[i - 8] = timeSum;
	}
	//printf("%d\n", timeSum);
	return timesSums;
}

int* getNewestTrueRRPeakValues(){
	return &trueRRPeakVal[8];
}
//TODO check this method
void recordNewProperRPeak(Peak newPeak){
	//Calulates the new values for determining if a peak is an RR peak:
	insertAvgCircData(&threshold1PassPeaks,newPeak);
	Spkf = newPeak.intensity/8 + 7*Spkf/8;
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
	RR_Low = 23*RR_AVERAGE2/25; /*23/25= 0.92*/
	RR_High = 29*RR_AVERAGE2/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_AVERAGE2/50; /*83/50 = 1.66*/

	/*TODO Discuss Andreas, what if the RR interval becomes large enough that multiplying by 83 makes an overflow error,
	 * For example in the case of a searchback?
	 * */

	//The peak is registrated as a true RR peak.
	insertPeakCircArrayData(&trueRRPeaks,newPeak);
}

int backwardCircularArray(int size, int currentIndex, int offset){
	currentIndex += offset;
	return ( (currentIndex  < 0)? currentIndex + size: currentIndex);

}

Peak getPeakAnyPeak(int offsetCurrent){
	return (allPeaksRR[backwardCircularArray(SIZE_R_ARRAYS, indexAllPeaks, offsetCurrent)]);
}

int registerSearchBackPeak(Peak peak){

	/*When it gets classified as an R-peak, what must be done?
	 *  See removing from the average.
	 *  What if it is an already seen R-peak?
	 *  TODO
	   */

	//Calulates the new values for determining if a peak is an RR peak:
	Spkf = peak.intensity/4 + 3*Spkf/4;
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
	RR_Low = 23*RR_Average1/25; /*23/25= 0.92*/
	RR_High = 29*RR_Average1/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_Average1/50; /*83/50 = 1.66*/

	/*Recording it as an proper R-peak. Will always be later than the current RPeaks*/
	/*Also updating RR_Average_2, despite the flowchart, so it can be done one by one as they come,
	 * instead of running through the array	 * */
	insertPeakCircArrayData(&trueRRPeaks,peak);
	/*TODO Ask. Should the later RR values be updated so that this is the latest RR peak?*/
	return 1;
}

int searchBackBackwardsGoer(int indexMiss){
	//The lower the value, the more likely is it to be a true peak.

	int minInverseLikelyhoodIsTruePeak = INT_MAX;
	Peak* indexMaxLikelyhoodIsTruePeak;
	int currentInverseLikelyhood = 0;

	/*TODO Don't know the average currently used. Using number two currently*/
	/*TODO Discuss with Andreas, up to the missed peak
	 * TODO check for pointer error after done*/
	for(int i = 0; i <= indexMiss; i++){
		if(tempPeaksForSearchback[i].intensity > Threshold2){
			if(tempPeaksForSearchback[i].RR <= RR_Low)
				currentInverseLikelyhood = RR_AVERAGE2 - tempPeaksForSearchback[i].RR;
			else
				currentInverseLikelyhood = (tempPeaksForSearchback[i].RR - RR_AVERAGE2)/2;
			if(currentInverseLikelyhood <= minInverseLikelyhoodIsTruePeak){
				minInverseLikelyhoodIsTruePeak = currentInverseLikelyhood;
				indexMaxLikelyhoodIsTruePeak = &tempPeaksForSearchback[i];
			}
		}
	}
	//Returns the index to the most likely-to-be-a-proper-R-peak peak.
	//There will always be one, as the peak triggering the searchback is a candidate.

	//It registrates the peak:
	registerSearchBackPeak(*indexMaxLikelyhoodIsTruePeak);
	return indexMaxLikelyhoodIsTruePeak;
}

int searchBack(){
	//TODO Set the RR value in the peakFinder algorithm, depending on what is registrated.

	int indexMostBackwards = searchBackBackwardsGoer(tempIndexPeaksForSearchback);
	//For decreasing the value of the RR, since the now last reigstrated peak;
	int newRRRemoval = tempPeaksForSearchback[indexMostBackwards].RR;
	//TODO fix indexMostBackwards after tests.
	int i = indexMostBackwards+1;
	/*i needs to be less than tempIndexPeaksForSearchback, as it is 1 greater the RR_MISS peak*/
	for(; i < tempIndexPeaksForSearchback; i++){
		//TODO dicuss with Andreas, hardcopy and any error that might occour because of that.
		Peak currentPeak = tempPeaksForSearchback[i];
		currentPeak.RR -= newRRRemoval;
		/*The data is moved back in the array, so the peaks have the right position to the last true RR peak found,
		 *when finished going over the array.
		   */
		tempPeaksForSearchback[i - indexMostBackwards - 1] = currentPeak;
		/*TODO Make it so that the last Threshold 1 registered is not overwritten, or that they are registrated twice,
		 *see for example the peak that triggered it all*/
		if(passThreshold1(currentPeak)){
			if ((RR_Low < currentPeak.RR && currentPeak.RR < RR_High)){
				/*TODO Something wrong here?*/
				recordNewProperRPeak(currentPeak);
				newRRRemoval += currentPeak.RR;
				indexMostBackwards = i;
			} else if(currentPeak.RR > RR_Miss){
				/*It might happen that because of the change in RR_Miss because of the registration of a new peak,
				 * That a peak that for example where 165 percent of the peak estimation/average, becomes 170,
				 * thus triggering another miss. This is handled somewhat the same here,
				 * restricting the searchback to a certain part of the peak array.
				 * */
				//TODO Ask the teacher, why this fixes the weird problem. Might be fixed again=D.
				int temptemptemp = i - indexMostBackwards - 1;
				indexMostBackwards += searchBackBackwardsGoer(temptemptemp) + 1;
				/*searchBackBackwardsGoer(i - indexMostBackwards) as a peak at position i in the array.
				 *have been moved to position i - indexMostBackwards, if the position exists.
				   */
				i = indexMostBackwards;
				//TODO wrong update. Maybe fixed?
				newRRRemoval += currentPeak.RR;
			}
		}
	}
	/*indexMostBackwards is the position of the last registrated RR-peak */
	tempIndexPeaksForSearchback -= indexMostBackwards + 1;
	return 1;

}

int passThreshold1(Peak peak){
	/*TODO Discuss with the teacher.
		 * Does RR_Average1 only have to updated if (RR_Low < peakTime_0 && peakTime_0 < RR_High)==True below?.
		  *  For this is not true if it is false, and the searchback is runned.
		 * Don't Remove - Jesper
		 * */
		/*Written in that way, so it looks more like the flow chart.*/
	if (!(peak.intensity > Threshold1)) { /*If it isn't an R-Peak..*/
			Npkf = peak.intensity/AVERAGE_NUMBER_MEMBERS + 7*Npkf/8;
			Threshold1 = Npkf + (Spkf-Npkf)/4;
			Threshold2 = Threshold1/2;
			/*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
			addRRTimeFromFormer();
			return (0);
		} else{
			/*Now it is classified as an R-peak, but not neccesarily a proper one. It is recorded as such.*/

			insertAvgCircData(&threshold1PassPeaks,peak);
			return 1;
		}
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
int isRPeak(Peak newPeak){ /*peakTime=RR*/
	/*Checks if it is an RPeak*/
	allPeaksVal[indexAllPeaks] = newPeak.intensity;
	allPeaksRR[indexAllPeaks] = newPeak.RR;
	indexAllPeaks++; /*TODO Change. Is a temp solution for testing.*/
	//TODO remove code below after finishing searchback.
	tempPeaksForSearchback[tempIndexPeaksForSearchback] = newPeak;
	tempIndexPeaksForSearchback++;

	//printf("RR value is %d, RR_LOW=%d and RR_HIGH=%d \n",peakTime_0, RR_Low,RR_High);

	/*At any time, RR_Average_1 is calculated,
	 *so it is not necessary to actually run over the 8 last peak values every time.
	   */
	RR_Average1_Sum = RR_Average1_Sum + newPeak.RR - getPeakAnyPeak(-7).RR;
	//TODO replace the method  getPeakTimeAnyPeak(-7) with the array method
	RR_Average1 = RR_Average1_Sum/8;

	return rPeakChecks(newPeak,getPeakTimeAnyRPeak(-7));
}

/*
 *
 */
int rPeakChecks(Peak newPeak){
	if (passThreshold1(newPeak)){
		/*If it is in the RR-interval*/
		if (RR_Low < newPeak.RR && newPeak.RR < RR_High){
			recordNewProperRPeak(newPeak);
			//Sets it so that there have been no new peaks found, since this RR peak.
			tempIndexPeaksForSearchback = 0;
			return (1);
		} else if(newPeak.RR > RR_Miss){
			return (searchBack());
		} else{
			 /*Makes it so that the RR value for the later peaks,
			  *doesn't take this as an RR peak.*/
			addRRTimeFromFormer();
			return (0);
		}
	} else return 0;
}

int forwardCircularArray(int size, int currentIndex, int offset){
	currentIndex += offset;
	if (currentIndex >= size) return (currentIndex - size);
	else return (currentIndex);
}

PeakCircularArray getTrueRPeaksArray(){
	return trueRRPeaks;
}


