#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peakSearcher.h"
#define SIZE_R_ARRAYS 128

/*Storing*/
int rPeaks[SIZE_R_ARRAYS]; /*TODO make circular*/
int RRIntervalOk[SIZE_R_ARRAYS]; /*TODO make circular*/

/*Indexes for the storing*/
int indexRPeaks = 0; /*Can be used for both rPeaks and RRInterval*/

/*Variables for detecting R peaks*/
int Spkf = 0;
int Npkf = 0;
int Threshold1 = 0;
int Threshold2 = 0;

/*Variables for finding the RR-interval.*/
RR_Average1_Sum = 0;
RR_Average2_Sum = 0;
RR_Average1 = 0;
RR_Average2 = 0;
RR_Low = 0;  /*TODO, check if there is a possibility of the values becoming "locked"*/
RR_High = 0;
RR_Miss = 0;

/*Variables used for the searchback*/
/*TODO make searchback.*/

int isRPeak(int peakValue, int peakTime_0, int peakTime_7){ /*peakTime=RR*/
	/*Writen in that way, so it looks more like the flow chart.*/
	/*Checks if it is an RPeak*/

	int RR = 0;

	/*TODO Discuss with the teacher.
	 * Does RR_Average1 only have to updated if (RR_Low < peakTime_0 && peakTime_0 < RR_High)==True below?.
	  *  For this is not true if it is false, and the searchback is runned.
	 * Don't Remove - Jesper
	 * */
	RR_Average1_Sum = RR_Average1_Sum + peakTime_0 - peakTime_7;
	RR_Average1 = RR_Average1_Sum/8;

	if (!(peakValue < Threshold1)) { /*If it isn't an R-Peak..*/
		Npkf = peakValue/8 + 7*Npkf/8;
		Threshold1 = Npkf + (Spkf-Npkf)/4;
		Threshold2 = Threshold1/2;
		return (0);
	}
	/*If it is in the RR-interval*/
	if (RR_Low < peakTime_0 && peakTime_0 < RR_High){
		recordNewProperRPeak(peakValue, peaktTime_0, peakTime_7);
	} else if(peakTime < RR_Miss){
		runSearchback();
	}
}

void recordNewProperRRPeak(int peakValue, int peakTime_0, int peakTime_7){
	RR_Average2_Sum = RR_Average2_Sum + peakTime_0 - backwardCircularArray(SIZE_R_ARRAYS, indexRPeaks, -7); /*Seven back because the current peak hasn't been recurded*/
	RR_Average2 = RR_Average2_Sum/8;
	RRIntervalOk[indexRPeaks] = peakTime_0;
	indexRPeaks = forwardCircularArray(SIZE_R_ARRAYS, indexRPeaks, 1);
	RR_Low = 23*RR_Average2_Sum/25; /*23/25= 0.92*/
	RR_High = 29*RR_Average2_Sum/25; /*29/25= 1.16*/
	RR_Miss = 83*RR_Average2_Sum/50; /*83/50 = 1.66*/
	/*TODO Depending on the occurence of the below check, it might pay to not calculate it here at all.*/
	Spkf = peakValue/8 + 7*Spkf/8;
	Threshold1 = Npkf + (Spkf-Npkf)/4;
	Threshold2 = Threshold1/2;
}

int backwardCircularArray(int size, int currentIndex, int offset){
	currentIndex += offset;
	return ( (currentIndex  < 0)? currentIndex + size: currentIndex);

}

int forwardCircularArray(int size, int currentIndex, int offset){
	currentIndex += offset;
	return ( (currentIndex => size)? currentIndex - size: currentIndex);

}
