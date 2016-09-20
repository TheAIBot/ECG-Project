#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/avgCircularArray.h"

int getCorrectAvgCircIndex(int startIndex, int offset, int sizeArray){
	int correctIndex = startIndex + offset;
	if (correctIndex < 0)
		correctIndex += sizeArray;
	else if (correctIndex >= sizeArray)
		correctIndex -= sizeArray;
	return correctIndex;
}

void setAvgCircValue(struct TAvgCircularArray* avgCirc, const int offset, const Peak peak){
	int correctIndex = getCorrectAvgCircIndex(avgCirc->startIndex,offset, avgCirc->averageLength);
	avgCirc->averageSum += peak.RR - avgCirc->data[correctIndex].RR;
	avgCirc->data[correctIndex] = peak;
}

Peak getAvgCircValue(const struct TAvgCircularArray* avgCirc, const int offset){
	int correctIndex = avgCirc->startIndex + offset;
	if(correctIndex < 0){
		correctIndex += avgCirc->size;
	} else if (correctIndex >= avgCirc->size){
		correctIndex -= avgCirc->size;
	}
	return avgCirc->data[correctIndex];
}

static inline void moveAvgCircIndexForward(AvgCircularArray* avgCirc)
{
	avgCirc->startIndex++;
	if(avgCirc->startIndex == avgCirc->size)	{
		avgCirc->startIndex = 0;
	}
}

void insertAvgCircData(AvgCircularArray* avgCirc, const Peak newData){
	moveAvgCircIndexForward(avgCirc);
	avgCirc->averageSum += newData.RR - getAvgCircValue(avgCirc, -avgCirc->averageLength).RR;//avgCirc->data[avgCirc->averageLength];
	(avgCirc->data)[avgCirc->startIndex]= newData;
}

int getAvgCircAverageRR(const struct TAvgCircularArray* avgCirc)
{
	return avgCirc->averageSum / avgCirc->averageLength;
}

//TODO is it correct after using peaks?
void resetAvgCirc(struct TAvgCircularArray* avgCirc)
{
	memset(avgCirc->data, 0, avgCirc->size * sizeof(int));
	avgCirc->averageSum = 0;
}

void freeAvgCirc(struct TAvgCircularArray* avgCirc)
{
	free(avgCirc->data);
}

