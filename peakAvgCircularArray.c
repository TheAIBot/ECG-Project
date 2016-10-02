#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/peakAvgCircularArray.h"

/* Initializes a new avgCirc (pointer), filling it up with a given default Peak, defaultPeak,
 * setting its size to a given size, and placing the current index at a given value startIndex.
 * Returns 1 if it works, 0 otherwise.
 *
 * PeakAvgCircularArray* avgCirc; the pointer to the PeakAvgCircularArray.
 * int size; the size of the PeakAvgCircularArray.
 * int startIndex; the current index (to be set) of the  PeakAvgCircularArray.
 * Peak defaultPeak; the Peak to fill the array up with.
 * */
char initPeakAvgCircArray(PeakAvgCircularArray* const peakAvgCirc, const int size, const int startIndex, const Peak defaultPeak){
	if(!initCircArray(&(peakAvgCirc->circArray), size, startIndex))
	{
		return 0;
	}

	peakAvgCirc->averageSum = defaultPeak.RR * size;

	for(int i = 0; i < peakAvgCirc->circArray.size; i++)
	{
		peakAvgCirc->circArray.data[i].peak = defaultPeak;
	}

	return 1;
}

Peak getPeakAvgCircValue(const PeakAvgCircularArray* peakAvgCirc, const int offset){
	return getCircArrayPeakOrInt(&(peakAvgCirc->circArray), offset).peak;
}

void insertPeakAvgCircData(PeakAvgCircularArray* const peakAvgCirc, const Peak newPeak){
	moveCircArrayStartIndex(&(peakAvgCirc->circArray));
	//The value to be deleted is the value one is currently at:
	peakAvgCirc->averageSum += newPeak.RR- peakAvgCirc->circArray.data[peakAvgCirc->circArray.startIndex].peak.RR;
	peakAvgCirc->circArray.data[peakAvgCirc->circArray.startIndex].peak = newPeak;
}

int getPeakAvgCircAverageValue(const PeakAvgCircularArray* peakAvgCirc){
	return getAvgCircAverageValue(peakAvgCirc);
}

void resetPeakAvgCirc(PeakAvgCircularArray*  const peakAvgCirc){
	resetAvgCirc(peakAvgCirc);
}

void freePeakAvgCirc(PeakAvgCircularArray* const peakAvgCirc){
	freeCircArray(&(peakAvgCirc->circArray));
}

