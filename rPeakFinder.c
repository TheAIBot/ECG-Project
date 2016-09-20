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
#define RR_AVERAGE1 getAvgCircAverageRR(&allPeaks)
#define RR_AVERAGE2 getAvgCircAverageRR(&threshold1PassPeaks)
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


PeakCircularArray trueRPeaks; //Last eight true r peak
AvgCircularArray threshold1PassPeaks; //Last eight peaks which intensity surpassed Threshold1 (making them R peaks)
AvgCircularArray allPeaks;//Last eight peaks found, no matter what.
/*TODO Andreas, what about freeing the memory of the peaks?*/
Peak * tempPeaksForSearchback[16]; //Last peaks since last register of a true R peak. Used in searchback.
int tempIndexPeaksForSearchback = 0; //Index of tempPeaksForSearchback, pointing to the next free space.

/*Variables for detecting R peaks*/
int Spkf = 3500;
int Npkf = 300;
//TODO Might want to make to macros. Depends on performance.
int Threshold1 =  1100; /*Answers to: Npkf + (Spkf-Npkf)/4*/
int Threshold2 = 550; /*Answers to Threshold1/2*/

/*Variables for finding the RR-interval
 * Given as though calculated from RR_Average2
 *TODO What about macros? But problems.
 *TODO should it be from RR_Average1 or 2 at the beggining. What are they?
  */
int RR_Low = 138;  /*TODO, check if there is a possibility of the values becoming "locked"*/
int RR_High = 173;
int RR_Miss = 249;

void initializeRPeakFinder()
{
	/* Looking from the data, the intensity of a normal heart beat is at least 3500.
	 * Looking at the Internet, see http://www.heart.org/HEARTORG/HealthyLiving/PhysicalActivity/Target-Heart-Rates_UCM_434341_Article.jsp
	 * the average resting heat rate, the time between an heart beat, and thus also the RR-interval, is around 60-100 bpm.
	 * Taking 100 as the average, with 250 measurements in a second, it corresponds to about 150 measurements per heartbeat.
	 */
	const Peak defaultTrueRPeak = {.RR = 150, .intensity = 3500};
	/*Simply setting it to the same as above, as it is close enough*/
	const Peak defaultThreshold1Peak = {.RR = 150, .intensity = 3500};
	/*Lets say that one average there is detected one true R peak per non-R-peak detected, and let the later have an
	 * Average RR value of 50 and an intensity of 300. The the Average RR value is (50 + 150 /2)=100,
	 * and the average intensity is (3500 + 300)/2 = 1800
	 * */
	const Peak defaultAllPeak = {.RR = 100, .intensity = 1800};
	initPeakCircArray(&trueRPeaks, AVERAGE_NUMBER_MEMBERS, 0, defaultTrueRPeak);
	initAvgCircArray(&allPeaks, AVERAGE_NUMBER_MEMBERS, 0, AVERAGE_NUMBER_MEMBERS, defaultAllPeak);
	initAvgCircArray(&threshold1PassPeaks, AVERAGE_NUMBER_MEMBERS, 0, AVERAGE_NUMBER_MEMBERS, defaultThreshold1Peak);

}

//TODO check this method
void recordNewProperRPeak(Peak* newPeak){
	//Calulates the new values for determining if a peak is an RR peak:
	Spkf = newPeak->intensity/8 + 7*Spkf/8;
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
	RR_Low = 23*RR_AVERAGE2/25; /*23/25= 0.92*/
	RR_High = 29*RR_AVERAGE2/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_AVERAGE2/50; /*83/50 = 1.66*/
	/*TODO Discuss Andreas, what if the RR interval becomes large enough that multiplying by 83 makes an overflow error,
	 * For example in the case of a searchback?
	 * */
	//The peak is registrated as a true RR peak.
	insertPeakCircArrayData(&trueRPeaks, newPeak);
}

int registerSearchBackPeak(Peak* peak){

	/*When it gets classified as an R-peak, what must be done?
	 *  See removing from the average.
	 *  What if it is an already seen R-peak?
	 *  TODO
	   */
	//Calulates the new values for determining if a peak is an RR peak:
	Spkf = peak->intensity/4 + 3*Spkf/4;
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
	RR_Low = 23*RR_AVERAGE1/25; /*23/25= 0.92*/
	RR_High = 29*RR_AVERAGE1/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_AVERAGE1/50; /*83/50 = 1.66*/

	/*Recording it as an proper R-peak. Will always be later than the current RPeaks*/
	insertPeakCircArrayData(&trueRPeaks, peak);
	/*TODO Ask. Should the later RR values be updated so that this is the latest RR peak?*/
	return 1;
}

int searchBackBackwardsGoer(int indexMiss){
	//The lower the value, the more likely is it to be a true peak.
	int minInverseLikelyhoodIsTruePeak = INT_MAX;
	int indexMaxLikelyhoodIsTruePeak;
	int currentInverseLikelyhood = 0;

	/*TODO Don't know the average currently used. Using number two currently*/
	/*TODO Discuss with Andreas, up to the missed peak
	 * TODO check for pointer error after done*/
	for(int i = 0; i <= indexMiss; i++){
		if(tempPeaksForSearchback[i]->intensity > Threshold2){
			if(tempPeaksForSearchback[i]->RR <= RR_Low)
				currentInverseLikelyhood = RR_AVERAGE2 - tempPeaksForSearchback[i]->RR;
			else
				currentInverseLikelyhood = (tempPeaksForSearchback[i]->RR - RR_AVERAGE2)/2;
			if(currentInverseLikelyhood <= minInverseLikelyhoodIsTruePeak){
				minInverseLikelyhoodIsTruePeak = currentInverseLikelyhood;
				indexMaxLikelyhoodIsTruePeak = i;
			}
		}
	}
	//It records the peak:
	registerSearchBackPeak(tempPeaksForSearchback[indexMaxLikelyhoodIsTruePeak]);

	//Returns the index to the most likely-to-be-a-proper-R-peak peak.
	//There will always be one, as the peak triggering the searchback is a candidate.

	return indexMaxLikelyhoodIsTruePeak;
}

int searchBack(){
	//TODO Set the RR value in the peakFinder algorithm, depending on what is registrated.
	//TODO check if -1 below is correct
	int indexMostBackwards = searchBackBackwardsGoer(tempIndexPeaksForSearchback - 1);
	//For decreasing the value of the RR, since the now last registrated peak;
	int newRRRemoval = tempPeaksForSearchback[indexMostBackwards]->RR;
	//TODO fix indexMostBackwards after tests.
	int i = indexMostBackwards+1;
	/*i needs to be less than tempIndexPeaksForSearchback, as it is 1 greater the RR_MISS peak*/
	for(; i < tempIndexPeaksForSearchback; i++){
		Peak* currentPeak = tempPeaksForSearchback[i];
		currentPeak->RR -= newRRRemoval;
		/*The data is moved back in the array, so the peaks have the right position to the last true RR peak found,
		 *when finished going over the array.
		   */
		tempPeaksForSearchback[i - indexMostBackwards - 1] = currentPeak;
		/*TODO Make it so that the last Threshold 1 registered is not overwritten, or that they are registrated twice,
		 *see for example the peak that triggered it all*/
		if(passThreshold1(currentPeak)){
			if ((RR_Low < currentPeak->RR && currentPeak->RR < RR_High)){
				recordNewProperRPeak(currentPeak);
				/* Since a new true R peak has been found, it is necessary to add it's current RR value to newRRRemoval,
				 * so the RR value of the later peaks become the RR value from the currently found true R peak,
				 * when newRRRemoval has been removed from there value.
				 */
				newRRRemoval += currentPeak->RR;
				indexMostBackwards = i;
			} else if(currentPeak->RR > RR_Miss){
				/* It might happen that for example the peak found to be the closest to a true R-peak,
				 * have an value RR value so low, that the RR value of the peak that triggered the searchback,
				 * even after it's registration, is above RR_Miss -> in such cases, amongst others,
				 * another searchback is triggered.
				  */
				//TODO Ask the teacher, why this fixes the weird problem. Might be fixed again=D.
				int temptemptemp = i - indexMostBackwards - 1;
				indexMostBackwards += searchBackBackwardsGoer(temptemptemp) + 1;
				/*It is "searchBackBackwardsGoer(i - indexMostBackwards)" as a peak at position i in the array.
				 *have been moved to position i - indexMostBackwards, if the position exists.
				   */
				i = indexMostBackwards;
				//TODO wrong update. Maybe fixed?
				newRRRemoval += currentPeak->RR;
			}
		}
	}
	/*indexMostBackwards is the position of the last registrated RR-peak */
	tempIndexPeaksForSearchback -= indexMostBackwards + 1;
	return 1;

}

int passThreshold1(Peak* peak){
	/*TODO Discuss with the teacher.
		 * Does RR_Average1 only have to updated if (RR_Low < peakTime_0 && peakTime_0 < RR_High)==True below?.
		  *  For this is not true if it is false, and the searchback is runned.
		 * Don't Remove - Jesper
		 * */
		/*Written in that way, so it looks more like the flow chart.*/
	if (!(peak->intensity > Threshold1)) { /*If it isn't an R-Peak..*/
			Npkf = peak->intensity/AVERAGE_NUMBER_MEMBERS + 7*Npkf/8;
			Threshold1 = Npkf + (Spkf-Npkf)/4;
			Threshold2 = Threshold1/2;
			/*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
			addRRTimeFromFormer();
			return (0);
		} else{
			/*Now it is classified as an R-peak, but not neccesarily a proper one. It is recorded as such.*/
			insertAvgCircData(&threshold1PassPeaks, peak);
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
int isRPeak(Peak* newPeak){
	insertAvgCircData(&allPeaks,newPeak);
	//allPeaks Move one forward to fast, but still fine. TODO


	tempPeaksForSearchback[tempIndexPeaksForSearchback] = newPeak;
	tempIndexPeaksForSearchback++;
	return rPeakChecks(newPeak);
}

/*
 *
 */
int rPeakChecks(Peak* newPeak){
	if (passThreshold1(newPeak)){
		/*If it is in the RR-interval*/
		if (RR_Low < newPeak->RR && newPeak->RR < RR_High){
			recordNewProperRPeak(newPeak);
			//Sets it so that there have been no new peaks found, since this RR peak.
			tempIndexPeaksForSearchback = 0;
			return (1);
		} else if(newPeak->RR > RR_Miss){
			return (searchBack());
		} else{
			 /*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
			addRRTimeFromFormer();
			return (0);
		}
	} else return 0;
}

PeakCircularArray* getTrueRPeaksArray(){
	return &trueRPeaks;
}

//TODO check this method
int* getNewestTrueRRPeakTimes(int timesCount){
	/*
	int* timesSums = malloc(timesCount * sizeof(int));
	int timeSum = 0;
	for(int i = 8; i < timesCount + 8; i++)
	{
		timeSum += trueRRPeakRR[i];
		timesSums[i - 8] = timeSum;
	}
	//printf("%d\n", timeSum);
	 *
	 */
	return malloc(31 * sizeof(int));
}

int* getNewestTrueRRPeakValues(){
	return malloc(31 * sizeof(int));
}

PeakCircularArray* getTrueRPeaks(){
	return &trueRPeaks;
}

