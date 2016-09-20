#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/avgCircularArray.h"

char initAvgCircArray(AvgCircularArray* avgCirc, int size, int startIndex, int averageLength, Peak defaultPeak)
{
	avgCirc->size = size;
	avgCirc->startIndex = startIndex;
	avgCirc->averageSum = defaultPeak.RR*size;
	avgCirc->averageLength = averageLength;
	avgCirc->data = malloc(avgCirc->size * sizeof(Peak*));

	if(avgCirc->data == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for average circular array");
		return 0;
	}

	for(int i = 0; i < avgCirc->size; i++){
		//TODO does it make a hard copy?
		Peak* copyDefaultPeak = malloc(sizeof(Peak));
		copyDefaultPeak->RR = defaultPeak.RR;
		copyDefaultPeak->intensity = defaultPeak.intensity;
		(avgCirc->data[i]) = copyDefaultPeak;
		printf("Address defaultPeak = %d, address copyDefaultPeak = %d\n", &defaultPeak, copyDefaultPeak);
	}
	avgCirc->data[2]->RR = 1200;
	printf("%d, %d\n", avgCirc->data[0]->RR, avgCirc->data[2]->RR);
	return 1;
}

static int getCorrectAvgCircIndex(int startIndex, int offset, int sizeArray){
	int correctIndex = startIndex + offset;
	if (correctIndex < 0)
		correctIndex += sizeArray;
	else if (correctIndex >= sizeArray)
		correctIndex -= sizeArray;
	return correctIndex;
}

void setAvgCircValue(struct TAvgCircularArray* avgCirc, const int offset, const Peak* peak){
	int correctIndex = getCorrectAvgCircIndex(avgCirc->startIndex,offset, avgCirc->averageLength);
	avgCirc->averageSum += peak->RR - avgCirc->data[correctIndex]->RR;
	avgCirc->data[correctIndex] = peak;
}

Peak* getAvgCircValue(const struct TAvgCircularArray* avgCirc, const int offset){
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

void insertAvgCircData(AvgCircularArray* avgCirc, const Peak* newPeak){
	moveAvgCircIndexForward(avgCirc);
	avgCirc->averageSum += newPeak->RR - getAvgCircValue(avgCirc, -avgCirc->averageLength)->RR;
	avgCirc->data[avgCirc->startIndex] = newPeak;
}

int getAvgCircAverageRR(const struct TAvgCircularArray* avgCirc)
{
	return avgCirc->averageSum / avgCirc->averageLength;
}

//TODO is it correct after using peaks?
void resetAvgCirc(struct TAvgCircularArray* avgCirc)
{
	memset(avgCirc->data, 0, avgCirc->size * sizeof(Peak*));
	avgCirc->averageSum = 0;
}

void freeAvgCirc(struct TAvgCircularArray* avgCirc)
{
	free(avgCirc->data);
}

