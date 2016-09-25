#include <stdlib.h>
#include <stdio.h>
#include "includes/circularArray.h"

/*Initializes a given PeakCircularArray (pointer), to have a given size and startindex.
 *
 * CircularArray* const circArray; Pointer to the circular array that should be initialized.
 * const int size; The size that the initialized array should have.
 * const int startIndex; The startindex that the initialized circular array should have.
 * */
char initCircArray(CircularArray* const circArray, const int size, const int startIndex){
	circArray->size = size;
	circArray->startIndex = startIndex;
	circArray->data = calloc(size, sizeof(int)); //Allocates space for the elements

	if(circArray->data == NULL)	{
		fprintf(stderr, "Failed to allocate memory for circular array\n");
		return 0;
	}
	return 1;
}

/* Gets the element in the given circular array (pointer), that has a given offset from the startindex.
 * The offset must be negative, and greater than or equal to minus the size of the circular array.
 *
 * returns The element with the given offset from the current/startindex.
 * */
int getCircArrayValue(const CircularArray* circArray, const int offset){
	return getArrayDataValue(circArray->data, circArray->startIndex, circArray->size, offset);
}

/* Gets the index to the element in the given circular array (pointer), that has a given offset from the startindex.
 * The offset must be negative, and greater than or equal to minus the size of the circular array.
 *
 * returns The index to the element with the given offset from the current/startindex.
 * */
static int getOffsettedIndex(const int startIndex, const int arraySize, const int offset){
	int correctIndex = startIndex + offset;
	//assumes that offset is always negative or 0
	if(correctIndex < 0){
		correctIndex += arraySize;
	}
	return correctIndex;
}

/* Gets the element in the given array, assuming it is the array contained in a circular array,
 * that has a given offset from the startindex.  The offset must be negative,
 * and greater than or equal to minus the size of the circular array.
 *
 * returns The element with the given offset from the current/startindex.
 * */
int getArrayDataValue(const int data[], const int startIndex, const int arraySize, const int offset){
	return data[getOffsettedIndex(startIndex, arraySize, offset)];
}

/*Moves one forward in the circular array. If it is at the end, it loops back to the start*/
static void moveCircArrayStartIndex(CircularArray* const circArray){
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size)	{
		circArray->startIndex = 0;
	}
}

/*Inserts a new element in a given circular array (pointer), at the position after the current startindex,
 *with a loopback if the startindex is at the last position in the array.
  *
 * CircularArray* const circArray; pointer for the new circular array, wherein the data point should be inserted.
 * const int newData; the element to be inserted.
  */
void insertCircArrayData(CircularArray* circArray, const int newData){
	moveCircArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

/*Frees the memory in the circular array*/
void freeCircArray(CircularArray* const circArray)
{
	free(circArray->data);
}








