#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "includes/peakSearcher.h"
#define PEAK_AREA 5
#define NUMBER_PEAKS_STORED 128
#define CURRENT_ALL_VAL allVal[currentAllValIndex]
#define FORMER_ALL_VAL(offset) allVal[formerPlaceCirkArray(PEAK_AREA, currentAllValIndex, offset)]
#define LATER_ALL_VAL(offset) allVal[nextPlaceCirkArray(PEAK_AREA, currentAllValIndex, offset)]

int currentTimeSinceLastPeak = -(PEAK_AREA - 1)/2; /*It starts at -(PEAK_AREA - 1)/2, because of the delay of (PEAK_AREA - 1)/2*/
int currentPeakIndex = 0;
int currentAllValIndex = 0;
/*All arrays are used as if they where circular.*/
int peaksVal[NUMBER_PEAKS_STORED];
int peaksTime[NUMBER_PEAKS_STORED];
int allVal[PEAK_AREA];

void testerPeak(){
	int kage[] = {20,19,18,23,22,26,18,2,50,60,60,20,3,3,3,3,3,3,3,3,3,3};
	int i = 0;
	for(; i < 21; i++){
		printf("for run number %d \n", i+1);
		searchPeak(kage[i]);
	}
	printf("done");
}

/*Finds if a data point is a peak, by checking if it is as large as or bigger than the two numbers before and after it.*/
void searchPeak(int newDataPoint){ /*A delay by two data points. Can be changed by changing the constant PEAK_AREA*/
	/*TODO Fix recording first data point as a peak.
	 *  Needs to be discussed with the teacher or the teaching assistants. - Jesper
	 */
	 /*Places the new data point two forwards from the current one.*/
	allVal[nextPlaceCirkArray(PEAK_AREA, currentAllValIndex, 2)] = newDataPoint;
	printf("%d \n", newDataPoint);
	if (isCurrentAPeak()){
		/*Records the value of the detected peak, and the time since the last peak.*/
		peaksTime[currentPeakIndex] = currentTimeSinceLastPeak;
		peaksVal[currentPeakIndex] = allVal[currentAllValIndex];
		printf("Peak is: %d \n",peaksVal[currentPeakIndex]);
		currentPeakIndex = nextIndexCirk(NUMBER_PEAKS_STORED, currentPeakIndex);
		currentTimeSinceLastPeak = 0;
	}
	currentAllValIndex = nextIndexCirk(PEAK_AREA, currentAllValIndex);
	currentTimeSinceLastPeak++;	
}

int isCurrentAPeak(){
	static int formerDifferentValue = 0;
	if (FORMER_ALL_VAL(-1) != CURRENT_ALL_VAL){ //To avoid registrating a peak in case of a fall, then a plateu, then a fall in values.
				formerDifferentValue = FORMER_ALL_VAL(-1);
			} else {
				printf("Not different, formerDifferent value is %d and former (by one) is %d \n", formerDifferentValue, FORMER_ALL_VAL(-1));
	}
	if (FORMER_ALL_VAL(-2) <= CURRENT_ALL_VAL){
		if (FORMER_ALL_VAL(-1) <= CURRENT_ALL_VAL){
			if (LATER_ALL_VAL(1) < CURRENT_ALL_VAL){
				if (LATER_ALL_VAL(2) < CURRENT_ALL_VAL){
					if (formerDifferentValue < CURRENT_ALL_VAL){
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int formerPlaceCirkArray(int size, int current, int offset){
	if (current + offset < 0){
			return (current + offset + size);
		} else  return (current + offset);
}

int nextPlaceCirkArray(int size, int current, int offset){
	if (current + offset >= size){
		return (current + offset - size);
	} else  return (current + offset);
}
int nextIndexCirk(int size, int current){
	if (current == (size - 1) ) return (0);
	else return (current+1);
}
