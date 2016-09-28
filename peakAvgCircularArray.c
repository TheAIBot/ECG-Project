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
	peakAvgCirc->size = size;
	peakAvgCirc->startIndex = startIndex;
	peakAvgCirc->averageSum = defaultPeak.RR * size;
	peakAvgCirc->data = malloc(peakAvgCirc->size * sizeof(Peak)); //Allocates memory for the array

	if(peakAvgCirc->data == NULL)	{
		fprintf(stderr, "Failed to allocate memory for average circular array");
		return 0;
	}

	for(int i = 0; i < peakAvgCirc->size; i++){
		peakAvgCirc->data[i] = defaultPeak;
	}
	return 1;
}

/* Gets the element in the Avg circular array (pointer), that has a given offset from the startindex.
 * The absolute value of the offset must be less than or equal to the size of the circular array.
  *
 * returns The peak with the given offset from the current/startindex.
 * */
Peak getPeakAvgCircValue(const PeakAvgCircularArray* peakAvgCirc, const int offset){
	int correctIndex = peakAvgCirc->startIndex + offset;
	//In the case that the index of the wanted element needs to loop around.
	//Requires that the absolute value of the offset is less than or equal to the size of the circular array.
	if(correctIndex < 0){
		correctIndex += peakAvgCirc->size;
	} else if (correctIndex >= peakAvgCirc->size){
		correctIndex -= peakAvgCirc->size;
	}
	return peakAvgCirc->data[correctIndex];
}

/*Moves the current index of a given PeakAvgCircularArray (pointer) one forwards.*/
static void movePeakAvgCircIndexForward(PeakAvgCircularArray* const peakAvgCirc){
	peakAvgCirc->startIndex++;
	if(peakAvgCirc->startIndex == peakAvgCirc->size)
		peakAvgCirc->startIndex = 0;
}

/* Moves the current index in a given PeakAvgCircularArray (pointer) one forwards,
 * and inserts the element there.
   *
 * PeakAvgCircularArray* avgCirc; pointer to the PeakAvgCircularArray.
 * const Peak newPeak; The peak to be inserted.
   *
   * */
void insertPeakAvgCircData(PeakAvgCircularArray* const peakAvgCirc, const Peak newPeak){
	movePeakAvgCircIndexForward(peakAvgCirc);
	//The value to be deleted is the value one is currently at:
	peakAvgCirc->averageSum += newPeak.RR- peakAvgCirc->data[peakAvgCirc->startIndex].RR;
	peakAvgCirc->data[peakAvgCirc->startIndex] = newPeak;
}

/*Gets the average RR value of the elements in a given PeakAvgCircularArray (pointer) peakAvgCirc
 *
 * const PeakAvgCircularArray* peakAvgCirc; pointer to the PeakAvgCircularArray.
 *
 * returns the average value of the RR value of the elements in the given PeakAvgCircularArray
   */
int getPeakAvgCircAverageValue(const PeakAvgCircularArray* peakAvgCirc){
	return peakAvgCirc->averageSum / peakAvgCirc->size;
}

/*Resets a given PeakAvgCircularArray, setting all the bytes to zero, and it's averageSum to zero*/
void resetPeakAvgCirc(PeakAvgCircularArray*  const peakAvgCirc){
	memset(peakAvgCirc->data, 0, peakAvgCirc->size * sizeof(Peak));
	peakAvgCirc->averageSum = 0;
}

/*Frees the given PeakAvgCircularArray (pointer)
  *
 * PeakAvgCircularArray* avgCirc; the pointer to the PeakAvgCircularArray.
 * */
void freePeakAvgCirc(PeakAvgCircularArray* const peakAvgCirc){
	free(peakAvgCirc->data);
}

