#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/avgCircularArray.h"

//TODO update description.
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
char initAvgCircArray(AvgCircularArray* const avgCirc, const int size, const int startIndex, const int defaultValue){
	avgCirc->size = size;
	avgCirc->startIndex = startIndex;
	avgCirc->averageSum = defaultValue * size;
	avgCirc->data = malloc(avgCirc->size * sizeof(int));

	if(avgCirc->data == NULL)	{
		fprintf(stderr, "Failed to allocate memory for average circular array");
		return 0;
	}

	for(int i = 0; i < avgCirc->size; i++){
		avgCirc->data[i] = defaultValue;
	}
	return 1;
}

/** Gets the value in a given AvgCircularArray (with a pointer to it),
 *  and an offset, offset, from the current index.
 *
 *  const AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
 *  const int offset; the offset from the current index.
 *
 *  returns a pointer to the Peak found.
 */
int getAvgCircValue(const AvgCircularArray* avgCirc, const int offset){
	//TODO OBS. Might not be used, can maybe be deleted.
	int correctIndex = avgCirc->startIndex + offset;
	if(correctIndex < 0){
		correctIndex += avgCirc->size;
	//TODO can remove this else?
	} else if (correctIndex >= avgCirc->size){
		correctIndex -= avgCirc->size;
	}
	return avgCirc->data[correctIndex];
}

/*Moves the current index of a given AvgCircularArray (pointer) one forwards.*/
static inline void moveAvgCircIndexForward(AvgCircularArray* const avgCirc){
	avgCirc->startIndex++;
	if(avgCirc->startIndex == avgCirc->size)
		avgCirc->startIndex = 0;
}

/* Moves the current index in a given AvgCircularArray (pointer) one forwards,
 * and inserts the pointer to a new Peak, newPeak.
   *
 * AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
 *  Peak* newPeak; pointer to the new Peak.
   *
   * */
void insertAvgCircData(AvgCircularArray* const avgCirc, const int newValue){
	moveAvgCircIndexForward(avgCirc);
	//The value to be deleted is the value one is currently at:
	avgCirc->averageSum += newValue - avgCirc->data[avgCirc->startIndex];
	avgCirc->data[avgCirc->startIndex] = newValue;
}

/*Gets the average value of a given AvgCircularArray (pointer) avgCirc
 *
 * const AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
   */
int getAvgCircAverageValue(const AvgCircularArray* avgCirc){
	return avgCirc->averageSum / avgCirc->size;
}
//TODO is it correct after using peaks?
void resetAvgCirc(AvgCircularArray*  const avgCirc){
	memset(avgCirc->data, 0, avgCirc->size * sizeof(int));
	avgCirc->averageSum = 0;
}
/*Frees the given AvgCircularArray (pointer)
  *
 * AvgCircularArray* avgCirc; the pointer to the AvgCircularArray.
 * */
void freeAvgCirc(AvgCircularArray* const avgCirc){
	free(avgCirc->data);
}

