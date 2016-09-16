#include <stdio.h>
#include "includes/avgCircularArray.h"

char initAvgCircArray(struct AvgCircularArray* avgCirc, int size, int startIndex, int avgLength)
{
	avgCirc->size = size;
	avgCirc->startIndex = startIndex;
	avgCirc->averageLength = avgLength;
	avgCirc->data = calloc(size, sizeof(int));

	if(avgCirc->data == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for circular array");
		return 0;
	}
	return 1;
}

int getAvgCircValue(struct AvgCircularArray* avgCirc, int offset)
{
	int correctIndex = avgCirc->startIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += avgCirc->size;
	}
	return avgCirc->data[correctIndex];
}

void insertAvgCircData(struct AvgCircularArray* avgCirc, int newData)
{
	moveAvgCircStartIndex(avgCirc);
	avgCirc->averageSum += newData - avgCirc->data[avgCirc->startIndex];
	avgCirc->data[avgCirc->startIndex] = newData;

}

int getAvgCircAverage(struct AvgCircularArray* avgCirc)
{
	return avgCirc->averageSum / avgCirc->averageLength;
}

void moveAvgCircStartIndex(struct AvgCircularArray* avgCirc)
{
	avgCirc->startIndex++;
	if(avgCirc->startIndex == avgCirc->size)
	{
		avgCirc->startIndex = 0;
	}
}

void freeAvgCirc(struct AvgCircularArray* avgCirc)
{
	free(avgCirc->data);
}

