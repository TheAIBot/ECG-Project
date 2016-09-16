#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peakSearcher.h"
#define SIZE_R_ARRAYS 128
#define AVERAGE_NUMBER_MEMBERS 8

/*Storing*/
/*TODO Explain why the different values*/
int trueRRPeakRR[SIZE_R_ARRAYS] = {[0 ... 7]=150};
int trueRRPeakVal[SIZE_R_ARRAYS]= {[0 ... 7]=3500};
int threshold1PassRR[SIZE_R_ARRAYS] = {[0 ... 7]=150};
int threshold1PassVal[SIZE_R_ARRAYS] = {[0 ... 7]=3500};
/* Suppose half the peaks found are noicepeaks with an average RR of 50, the other half are R-peaks,
 *,then the average of that is (50 + 150) / 2 = 100
   */
int allPeaksRR[SIZE_R_ARRAYS] = {[0 ... 7]=100}; /*TODO delete it. Only for testing purposes, else the same data is stored twice.*/
/* Suppose half the peaks found are noicepeaks with an average value of 300, the other half are R-peaks,
 *,then the average of that is (300 + 3500) / 2 = 1900
   */
int allPeaksVal[SIZE_R_ARRAYS] = {[0 ... 7]=1900}; /*TODO delete it. Only for testing purposes, else the same data is stored twice.*/

/*Indexes for the storing*/
int indexTrueRPeaks = 8;
int indexThreshold1PassPeak = 8;
int indexAllPeaks = 8; /*TODO delete it. Only for testing purposes*/
/*Variables for detecting R peaks*/
int Spkf = 3500;
int Npkf = 300;
int Threshold1 =  1100; /*Answers to: Npkf + (Spkf-Npkf)/4*/
int Threshold2 = 550; /*Answers to Threshold1/2*/

/*Variables for finding the RR-interval.*/
/*Given the values in the arrays above, one gets:*/
int RR_Average1_Sum = 8*100;
int RR_Average2_Sum = 8*150;
int RR_Average1 = 100;
int RR_Average2 = 150;
/*Given as though calculated from RR_Average1*/
int RR_Low = 138;  /*TODO, check if there is a possibility of the values becoming "locked"*/
int RR_High = 173;
int RR_Miss = 249;



int getNewestTrueRRPeakTime()
{
	int timeSum = 0;
	for(int i = 8; i < indexTrueRPeaks; i++)
	{
		timeSum += trueRRPeakRR[i];
	}
	//printf("%d\n", timeSum);
	return timeSum;
}

int getNewestTrueRRPeakValue()
{
	return trueRRPeakVal[indexTrueRPeaks - 1];
}

/*Variables used for the searchback*/

int tempPeaksForSearchbackRR[8]; //TODO Delete. Only temp.
int tempPeaksForSearchbackVal[8];//TODO Delete. Only temp.
int tempIndexPeaksForSearchback = 0;

void recordNewProperRPeak(int peakValue, int peakTime_0, int rPeakTime_7){
	Spkf = peakValue/8 + 7*Spkf/8;
	RR_Average2_Sum = RR_Average2_Sum + peakTime_0 - rPeakTime_7;
	RR_Average2 = RR_Average2_Sum/AVERAGE_NUMBER_MEMBERS;
	RR_Low = 23*RR_Average2/25; /*23/25= 0.92*/
	RR_High = 29*RR_Average2/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_Average2/50; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
	trueRRPeakVal[indexTrueRPeaks] = peakValue;
	trueRRPeakRR[indexTrueRPeaks] = peakTime_0;
	indexTrueRPeaks++; /*TODO Temp. To make circular*/
	tempIndexPeaksForSearchback = 0;
	/*TODO Depending on the occurrence of the below check, it might pay to not calculate it here at all.*/
}

int backwardCircularArray(int size, int currentIndex, int offset){
	currentIndex += offset;
	return ( (currentIndex  < 0)? currentIndex + size: currentIndex);

}

int getPeakTimeAnyRPeak(int offsetCurrent){
	return (threshold1PassRR[backwardCircularArray(SIZE_R_ARRAYS, indexThreshold1PassPeak, offsetCurrent)]);
}

int getPeakTimeAnyPeak(int offsetCurrent){
	return (allPeaksRR[backwardCircularArray(SIZE_R_ARRAYS, indexAllPeaks, offsetCurrent)]);
}

int checkSearchBack(int indexPeak){
	if (tempPeaksForSearchbackVal[indexPeak] > Threshold2){
		/*When it gets classified as an R-peak, what must be done?
		 *  See removing from the average.
		 *  What if it is an already seen R-peak?
		 *  TODO
		   */
		Spkf = tempPeaksForSearchbackVal[indexPeak]/4 + 3*Spkf/4;
		Threshold1 = Npkf + (Spkf-Npkf)/4;
		Threshold2 = Threshold1/2;
		RR_Low = 23*RR_Average1/25; /*23/25= 0.92*/
		RR_High = 29*RR_Average1/25; /*29/25= 1.16*/
		RR_Miss = 83*RR_Average1/50; /*83/50 = 1.66*/

		/*Recording it as an proper R-peak. Will always be later than the current RPeaks*/
		/*Also updating RR_Average_2, despite the flowchart, so it can be done one by one as they come,
		 * instead of running through the array	 * */
		RR_Average2_Sum = RR_Average2_Sum + tempPeaksForSearchbackRR[indexPeak] - getPeakTimeAnyRPeak(-7);
		RR_Average2 = RR_Average2_Sum/AVERAGE_NUMBER_MEMBERS;
		trueRRPeakVal[indexTrueRPeaks] = tempPeaksForSearchbackVal[indexPeak];
		trueRRPeakRR[indexTrueRPeaks] = tempPeaksForSearchbackRR[indexPeak];
		indexTrueRPeaks++; /*TODO Temp. To make circular*/
		/*TODO Ask. Should the later RR values be updated so that this is the latest RR peak?*/
		return 1;
	} else {
		return 0;
	}
}

int searchBack(){
	printf("Beginning searchback protocols\n");
	int i = tempIndexPeaksForSearchback - 2;
	int hasFoundProperNewPeak = 0;
	for(; i >= 0; i--){
		if (checkSearchBack(i)){
			hasFoundProperNewPeak = 1;
			break;
		}
	}
	/*If it has found a peak that can be taken as an R peak looking back through the array,
	 * It will check if some of the later peaks then are R peaks, if it is one.*/
	if(hasFoundProperNewPeak){
		/*Hack/Hijack of variable for efficiency*/
		hasFoundProperNewPeak = tempPeaksForSearchbackRR[i];
		i++;
		for(; i <= tempIndexPeaksForSearchback - 1; i++){
			tempPeaksForSearchbackRR[i] -= hasFoundProperNewPeak;
			if (checkSearchBack(i)){
				hasFoundProperNewPeak += tempPeaksForSearchbackRR[i];
			}
		}
		tempIndexPeaksForSearchback = 0;
		return 1;
	} else if (checkSearchBack(tempIndexPeaksForSearchback)){
		/*If it can't find any peaks that can be taken as an R,
		 * before the peak that has an RR value greater than RR-miss,
		 * it looks at that as an potential peak.*/
		tempIndexPeaksForSearchback = 0;
		return 1;
	} else{
		/*No proper search back peak found*/
		tempIndexPeaksForSearchback = 0;
		return 0;
	}
}

int isRPeak(int peakValue, int peakTime_0){ /*peakTime=RR*/
	/*Checks if it is an RPeak*/
	allPeaksVal[indexAllPeaks] = peakValue;
	allPeaksRR[indexAllPeaks] = peakTime_0;
	indexAllPeaks++; /*TODO Change. Is a temp solution for testing.*/
	int rPeakTime_7 = getPeakTimeAnyRPeak(-7);
	int peakTime_7 = getPeakTimeAnyPeak(-7);
	//TODO remove code below after finishing searchback.
	tempPeaksForSearchbackVal[tempIndexPeaksForSearchback] = peakValue;
	tempPeaksForSearchbackRR[tempIndexPeaksForSearchback] = peakTime_0;
	tempIndexPeaksForSearchback++;

	printf("RR value is %d, RR_LOW=%d and RR_HIGH=%d \n",peakTime_0, RR_Low,RR_High);

	/*At any time, RR_Average_1 is calculated,
	 *so it is not necessary to actually run over the 8 last peak values every time
	   */
	RR_Average1_Sum = RR_Average1_Sum + peakTime_0 - peakTime_7;
	RR_Average1 = RR_Average1_Sum/8;

	/*TODO Discuss with the teacher.
	 * Does RR_Average1 only have to updated if (RR_Low < peakTime_0 && peakTime_0 < RR_High)==True below?.
	  *  For this is not true if it is false, and the searchback is runned.
	 * Don't Remove - Jesper
	 * */
	/*Written in that way, so it looks more like the flow chart.*/
	if (!(peakValue > Threshold1)) { /*If it isn't an R-Peak..*/
		Npkf = peakValue/AVERAGE_NUMBER_MEMBERS + 7*Npkf/8;
		Threshold1 = Npkf + (Spkf-Npkf)/4;
		Threshold2 = Threshold1/2;
		/*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
		addRRTimeFromFormer();
		return (0);
	}
	/*Now it is classified as an R-peak, but not neccesarily a proper one. It is recorded as such.
	  */
	threshold1PassVal[indexThreshold1PassPeak] = peakValue;
	threshold1PassRR[indexThreshold1PassPeak] = peakTime_0;
	indexThreshold1PassPeak++; /*TODO make circular. Only temporary for testing*/

	/*If it is in the RR-interval*/
	if (RR_Low < peakTime_0 && peakTime_0 < RR_High){
		recordNewProperRPeak(peakValue, peakTime_0, rPeakTime_7);
		return (1);
	} else if(peakTime_0 > RR_Miss){
		return (searchBack());
	} else{
		addRRTimeFromFormer(); /*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
		return (0);
	}
}

int forwardCircularArray(int size, int currentIndex, int offset){
	currentIndex += offset;
	if (currentIndex >= size) return (currentIndex - size);
	else return (currentIndex);
}

