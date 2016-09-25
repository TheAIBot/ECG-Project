#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/peak.h"
#include "includes/avgCircularArray.h"

//TODO remember to write comments for the structs themself

/* Initializes a new AvgCircularArray avgCirc (pointer), filling it up with a given default value, defaultValue,
 * setting its size to a given size, and placing the current index at a given value startIndex.
 * Returns 1 if it works, 0 otherwise.
 *
 * AvgCircularArray* avgCirc; the pointer to the AvgCircularArray.
 * int size; the size of the AvgCircularArray.
 * int startIndex; the current index (to be set) of the  AvgCircularArray.
 * int defaultValue; the value to fill the array up with.
 *TODO set to shorts after checking it dosen't crash anything
 * */
char initAvgCircArray(AvgCircularArray* const avgCirc, const int size, const int startIndex, const int defaultValue){
	avgCirc->size = size;
	avgCirc->startIndex = startIndex;
	avgCirc->averageSum = defaultValue * size;
	avgCirc->data = malloc(avgCirc->size * sizeof(int)); //Allocates memory for the array

	if(avgCirc->data == NULL)	{
		fprintf(stderr, "Failed to allocate memory for average circular array");
		return 0;
	}

	for(int i = 0; i < avgCirc->size; i++){
		avgCirc->data[i] = defaultValue;
	}
	return 1;
}

/* Gets the element in the Avg circular array (pointer), that has a given offset from the startindex.
 * The absolute value of the offset must be less than or equal to the size of the circular array.
 *
 * returns The peak with the given offset from the current/startindex.
 * */
int getAvgCircValue(const AvgCircularArray* avgCirc, const int offset){
	//TODO OBS. Might not be used, can maybe be deleted. Same with the other ones.
	int correctIndex = avgCirc->startIndex + offset;
	//In the case that the index of the wanted element needs to loop around.
	//Requires that the absolute value of the offset is less than or equal to the size of the circular array.
	if(correctIndex < 0){
		correctIndex += avgCirc->size;
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
 * and inserts the element there.
   *
 * AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
 * const int newValue; The value to be inserted.
   *
   * */
void insertAvgCircData(AvgCircularArray* const avgCirc, const int newValue){
	moveAvgCircIndexForward(avgCirc);
	//The value to be deleted is the value one is currently at:
	avgCirc->averageSum += newValue - avgCirc->data[avgCirc->startIndex];
	avgCirc->data[avgCirc->startIndex] = newValue;
}

/*Gets the average value of the elements in a given AvgCircularArray (pointer) avgCirc
 *
 * const AvgCircularArray* avgCirc; pointer to the AvgCircularArray.
 *
 * returns the average value of the elements in the given AvgCircularArray
   */
int getAvgCircAverageValue(const AvgCircularArray* avgCirc){
	return avgCirc->averageSum / avgCirc->size;
}

/*Resets a given AvgCircularArray, setting all the values it contains to 0, and it's averageSum to zero*/
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

