#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peakSearcher.h"
#define SIZE_R_ARRAYS 128

/*Storing*/
int trueRRPeakRR[SIZE_R_ARRAYS];
int trueRRPeakVal[SIZE_R_ARRAYS];
int threshold1PassRR[SIZE_R_ARRAYS];
int threshold1PassVal[SIZE_R_ARRAYS];
int allPeaksVal[SIZE_R_ARRAYS]; /*TODO delete it. Only for testing purposes, else the same data is stored twice.*/
int allPeaksRR[SIZE_R_ARRAYS]; /*TODO delete it. Only for testing purposes, else the same data is stored twice.*/

/*Indexes for the storing*/
int indexTrueRPeaks = 0;
int indexThreshold1PassPeak = 0;
int indexAllPeaks = 0; /*TODO delete it. Only for testing purposes*/
/*Variables for detecting R peaks*/
int Spkf = 0;
int Npkf = 0;
int Threshold1 = 0;
int Threshold2 = 0;

/*Variables for finding the RR-interval.*/
int RR_Average1_Sum = 0;
int RR_Average2_Sum = 0;
int RR_Average1 = 0;
int RR_Average2 = 0;
int RR_Low = 0;  /*TODO, check if there is a possibility of the values becoming "locked"*/
int RR_High = 0;
int RR_Miss = 0;

/*Variables used for the searchback*/
/*TODO make searchback.*/

void recordNewProperRPeak(int peakValue, int peakTime_0, int rPeakTime_7){
	Spkf = peakValue/8 + 7*Spkf/8;
	RR_Average2_Sum = RR_Average2_Sum + peakTime_0 - rPeakTime_7;
	RR_Average2 = RR_Average2_Sum/8;
	RR_Low = 23*RR_Average2/25; /*23/25= 0.92*/
	RR_High = 29*RR_Average2/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_Average2/50; /*83/50 = 1.66*/
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
	trueRRPeakVal[indexTrueRPeaks] = peakValue;
	trueRRPeakRR[indexTrueRPeaks] = peakTime_0;
	indexTrueRPeaks++; /*TODO Temp. To make circular*/
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
	if (allPeaksVal[indexPeak] > Threshold2){
		/*When it gets classified as an R-peak, what must be done?
		 *  See removing from the average.
		 *  What if it is an already seen R-peak?
		 *  TODO
		   */
		Spkf = allPeaksVal[indexPeak]/4 + 3*Spkf/4;
		Threshold1 = Npkf + (Spkf-Npkf)/4;
		Threshold2 = Threshold1/2;
		RR_Low = 23*RR_Average1/25; /*23/25= 0.92*/
		RR_High = 29*RR_Average1/25; /*29/25= 1.16*/
		RR_Miss = 83*RR_Average1/50; /*83/50 = 1.66*/

		/*Recording it as an proper R-peak. Will always be later than the current RPeaks*/
		/*Alse updating RR_Average_2, despite the flowchart, so it can be done one by one as they come,
		 * instead of running through the array	 * */
		RR_Average2_Sum = RR_Average2_Sum + allPeaksRR[indexPeak] - getPeakTimeAnyRPeak(-7);
		RR_Average2 = RR_Average2_Sum/8;
		trueRRPeakVal[indexTrueRPeaks] = allPeaksVal[indexPeak];
		trueRRPeakRR[indexTrueRPeaks] = allPeaksRR[indexPeak];
		indexTrueRPeaks++; /*TODO Temp. To make circular*/
		/*TODO Ask. Should the later RR values be updated so that this is the latest RR peak?*/
		return 0;
	} else {
		return 0;
	}
}

int searchBack(){
	printf("Beginning searchback protocols");
	int i = indexAllPeaks - 1;
	for(; i >= 0; i--){
		if (checkSearchBack(i))
			return (1);
	}
	i = SIZE_R_ARRAYS - 1;
	for(; i != indexAllPeaks; i--){
		if (checkSearchBack(i))
			return (1);
	}
	/*No proper search back peak found*/
	return (0);
}

int isRPeak(int peakValue, int peakTime_0){ /*peakTime=RR*/
	/*Checks if it is an RPeak*/

	allPeaksVal[indexAllPeaks] = peakValue;
	allPeaksRR[indexAllPeaks] = peakTime_0;
	indexAllPeaks++; /*TODO Change. Is a temp solution for testing.*/
	int rPeakTime_7 = getPeakTimeAnyRPeak(-7);
	int peakTime_7 = getPeakTimeAnyPeak(-7);
	printf("RR value is %d, RR_LOW=%d and RR_HIGH=%d \n",peakTime_0, RR_Low,RR_High);

	/*At any time, RR_Average_1 is calculated. Might need to be moved.*/

	RR_Average1_Sum = RR_Average1_Sum + peakTime_0 - peakTime_7;
	RR_Average1 = RR_Average1_Sum/8;



	/*TODO Discuss with the teacher.
	 * Does RR_Average1 only have to updated if (RR_Low < peakTime_0 && peakTime_0 < RR_High)==True below?.
	  *  For this is not true if it is false, and the searchback is runned.
	 * Don't Remove - Jesper
	 * */
	/*Written in that way, so it looks more like the flow chart.*/
	if (!(peakValue > Threshold1)) { /*If it isn't an R-Peak..*/
		Npkf = peakValue/8 + 7*Npkf/8;
		Threshold1 = Npkf + (Spkf-Npkf)/4;
		Threshold2 = Threshold1/2;
		addRRTimeFromFormer(); /*Makes it so that the RR value for the later peaks, doesn't take this as an RR peak.*/
		return (0);
	}
	/*Now it is classified as an R-peak, but not neccesarily a proper one. It is recorded as such.*/

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
