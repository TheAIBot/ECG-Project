#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/avgCircularArray.h"

/* Initializes a new AvgCircularArray avgCirc (pointer), filling it up with a given default value, defaultValue,
 * setting its size to a given size, and placing the current index at a given value startIndex.
 * Returns 1 if it works, 0 otherwise.
 *
 * AvgCircularArray* avgCirc; the pointer to the AvgCircularArray.
 * int size; the size of the AvgCircularArray.
 * int startIndex; the current index (to be set) of the  AvgCircularArray.
 * int defaultValue; the value to fill the array up with.
 * */
char initAvgCircArray(AvgCircularArray* const avgCirc, const int size, const int startIndex, const int defaultValue){
	if(!initCircArray(&(avgCirc->circArray), size, startIndex))
	{
		return 0;
	}

	avgCirc->averageSum = defaultValue * size;

	for(int i = 0; i < avgCirc->circArray.size; i++)
	{
		avgCirc->circArray.data[i].integer = defaultValue;
	}

	return 1;
}

int getAvgCircValue(const AvgCircularArray* avgCirc, const int offset){
	return getCircArrayPeakOrInt(&(avgCirc->circArray), offset).integer;
}

void insertAvgCircData(AvgCircularArray* const avgCirc, const int newValue){
	moveCircArrayStartIndex(&(avgCirc->circArray));
	//The value to be deleted is the value one is currently at:
	avgCirc->averageSum += newValue - avgCirc->circArray.data[avgCirc->circArray.startIndex].integer;
	avgCirc->circArray.data[avgCirc->circArray.startIndex].integer = newValue;
}

int getAvgCircAverageValue(const AvgCircularArray* avgCirc){
	return avgCirc->averageSum / avgCirc->circArray.size;
}

void resetAvgCirc(AvgCircularArray*  const avgCirc){
	memset(avgCirc->circArray.data, 0, avgCirc->circArray.size * sizeof(PeakOrInt));
	avgCirc->averageSum = 0;
}

void freeAvgCirc(AvgCircularArray* const avgCirc){
	freeCircArray(&(avgCirc->circArray));
}

