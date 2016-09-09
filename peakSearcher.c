#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peakSearcher.h"
#define PEAK_AREA 5
#define NUMBER_PEAKS_STORED 128

int currentTimeSinceLastPeak = -(PEAK_AREA - 1)/2; /*It starts at -(PEAK_AREA - 1)/2, because of the delay of (PEAK_AREA - 1)/2*/
int currentPeakIndex = 0;
int currentAllValIndex = 0;
/*All arrays are used as if they where circular.*/
int peaksVal[NUMBER_PEAKS_STORED];
int peaksTime[NUMBER_PEAKS_STORED];
int allVal[PEAK_AREA];

void testerPeak(){
	int kage[] = {20,19,18,23,22,26,18,2,50,60,20,3,3,3};
	int i = 0;
	for(; i < 14; i++){
		printf("for run number %d \n", i+1);
		searchPeak(kage[i]);
	}
	printf("done");
}

/*Finds if a data point is a peak, by checking if it is as large as or bigger than the two numbers before and after it.*/
void searchPeak(int newDataPoint){ /*A delay by two data points. Can be changed by changing the constant PEAK_AREA*/
	 /*Places the new data point two forwards from the current one.*/
	allVal[nextPlaceCirkArray(PEAK_AREA, currentAllValIndex, 2)] = newDataPoint;
	int i = 0;
	for(; i < PEAK_AREA; i++){
		/*printf("%d < %d ?, ", allVal[currentAllValIndex], allVal[i]);
		printf((allVal[currentAllValIndex] < allVal[i]) ? "true" : "false");
		printf("\n");*/
		if(allVal[currentAllValIndex] < allVal[i])
			break;
	}
	if (i==PEAK_AREA){
		/*Records the value of the detected peak, and the time since the last peak.*/
		peaksTime[currentPeakIndex] = currentTimeSinceLastPeak;
		peaksVal[currentPeakIndex] = allVal[currentAllValIndex];
		currentPeakIndex = nextIndexCirk(NUMBER_PEAKS_STORED, currentPeakIndex);
		currentTimeSinceLastPeak = 0;
	}		
	currentAllValIndex = nextIndexCirk(PEAK_AREA, currentAllValIndex);
	currentTimeSinceLastPeak++;	
}
int nextPlaceCirkArray(int size, int current, int forward){
	if (current + forward >= size){
		return (current + forward - size);
	} else  return (current + forward);
}
int nextIndexCirk(int size, int current){
	if (current == (size - 1) ) return (0);
	else return (current+1);
}
