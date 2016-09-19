#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/avgCircularArray.h"

int getAvgCircValue(const struct AvgCircularArray* avgCirc, const int offset)
{
	int correctIndex = avgCirc->startIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += avgCirc->size;
	}
	return avgCirc->data[correctIndex];
}

static inline void moveAvgCircStartIndex(struct AvgCircularArray* avgCirc)
{
	avgCirc->startIndex++;
	if(avgCirc->startIndex == avgCirc->size)
	{
		avgCirc->startIndex = 0;
	}
}

void insertAvgCircData(struct AvgCircularArray* avgCirc, const int newData)
{
	moveAvgCircStartIndex(avgCirc);
	avgCirc->averageSum += newData - getAvgCircValue(avgCirc, -avgCirc->averageLength);//avgCirc->data[avgCirc->averageLength];
	avgCirc->data[avgCirc->startIndex] = newData;

}

int getAvgCircAverage(const struct AvgCircularArray* avgCirc)
{
	return avgCirc->averageSum / avgCirc->averageLength;
}

void resetAvgCirc(struct AvgCircularArray* avgCirc)
{
	memset(avgCirc->data, 0, avgCirc->size * sizeof(int));
	avgCirc->averageSum = 0;
}

void freeAvgCirc(struct AvgCircularArray* avgCirc)
{
	free(avgCirc->data);
}

