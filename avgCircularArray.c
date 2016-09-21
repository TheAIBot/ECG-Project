#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/avgCircularArray.h"

/* Initializes a new AvgCircularArray avgCirc (pointer), filling it up with a given Peak, defaultPeak,
 * setting its size to a given size, it's averageLength, and placing the current index at a given value startIndex.
 * Returns 1 if it works, 0 otherwise.
 *
 * AvgCircularArray* avgCirc; the pointer to the AvgCircularArray.
 * int size; the size of the AvgCircularArray.
 * int startIndex; the current index (to be set) of the  AvgCircularArray.
 * int averageLength; the averageLenght of the AvgCircularArray.
 * Peak defaultPeak; the Peak to fill the array up with.
 *
 * */
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
		Peak copDefPeak = defaultPeak;
		avgCirc->data[i] = &defaultPeak;


		/*
		Peak* copyDefaultPeak = malloc(sizeof(Peak));
		memcpy(copyDefaultPeak, &defaultPeak, sizeof(Peak));

		avgCirc->data[i] = copyDefaultPeak;
		*/
	}
	return 1;
}

/* Given an offset from the current index, it calculates the resulting index,
 * given that it is an index in an AvgCircularArray.
 * The offset must not be bigger than the size of the array.
   *
 *  int startIndex; the current index in the array.
 *  int offset; the offset from the startIndex.
 *  int sizeArray; the size of the circular array.
   *
 * returns the new index,
 *
 * */
static int getCorrectAvgCircIndex(int startIndex, int offset, int sizeArray){
	int correctIndex = startIndex + offset;
	if (correctIndex < 0)
		correctIndex += sizeArray;
	else if (correctIndex >= sizeArray)
		correctIndex -= sizeArray;
	return correctIndex;
}

/** Sets a Peak in the average circular array, with an given "offset" from the current index,
 *  to a given the address to a new Peak, peak.
 *
 *  AvgCircularArray* avgCirc; pointer to the AvgCircularArray where the peak should be set.
 *  const int offset; the offset from the current index.
 *  const Peak* peak; The address to the peak that should be inserted.
 *
 */
void setAvgCircValue(AvgCircularArray* avgCirc, const int offset, const Peak* peak){
	int correctIndex = getCorrectAvgCircIndex(avgCirc->startIndex,offset, avgCirc->averageLength);
	//It also updates the average, given the new insertion/deletion.
	avgCirc->averageSum += peak->RR - avgCirc->data[correctIndex]->RR;
	avgCirc->data[correctIndex] = peak;
}

/** Gets the value in a given AvgCircularArray (with a pointer to it),
 *  and an offset, offset, from the current index.
 *
 *  const AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
 *  const int offset; the offset from the current index.
 *
 *  returns a pointer to the Peak found.
 */
Peak* getAvgCircValue(const AvgCircularArray* avgCirc, const int offset){
	int correctIndex = avgCirc->startIndex + offset;
	if(correctIndex < 0){
		correctIndex += avgCirc->size;
	} else if (correctIndex >= avgCirc->size){
		correctIndex -= avgCirc->size;
	}
	return avgCirc->data[correctIndex];
}

/*Moves the current index of a given AvgCircularArray (pointer) one forwards.*/
static inline void moveAvgCircIndexForward(AvgCircularArray* avgCirc)
{
	avgCirc->startIndex++;
	if(avgCirc->startIndex == avgCirc->size)	{
		avgCirc->startIndex = 0;
	}
}

/*Moves the current index in a given AvgCircularArray (pointer) one forwards,
 * and inserts the pointer to a new Peak, newPeak.
   *
 * AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
 *  Peak* newPeak; pointer to the new Peak.
   *
   * */
void insertAvgCircData(AvgCircularArray* avgCirc, Peak* newPeak){
	moveAvgCircIndexForward(avgCirc);
	avgCirc->averageSum += newPeak->RR - getAvgCircValue(avgCirc, -avgCirc->averageLength)->RR;
	avgCirc->data[avgCirc->startIndex] = newPeak;
}

/*Gets the average value of a given AvgCircularArray (pointer) avgCirc
 *
 * const AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
   */
int getAvgCircAverageRR(const AvgCircularArray* avgCirc)
{
	return avgCirc->averageSum / avgCirc->averageLength;
}

void recalculateAverageAvgCircAverage(AvgCircularArray* avgCirc){
	avgCirc->averageSum = 0;
	for(int i = 0; i < avgCirc->averageLength; i++){
		avgCirc->averageSum += getAvgCircValue(avgCirc, -i)->RR;
	}
}

//TODO is it correct after using peaks?
void resetAvgCirc(struct TAvgCircularArray* avgCirc)
{
	memset(avgCirc->data, 0, avgCirc->size * sizeof(Peak*));
	avgCirc->averageSum = 0;
}
/*Frees the given AvgCircularArray (pointer)
  *
 * AvgCircularArray* avgCirc; the pointer to the AvgCircularArray.
 * */
void freeAvgCirc(AvgCircularArray* avgCirc)
{
	free(avgCirc->data);
}

