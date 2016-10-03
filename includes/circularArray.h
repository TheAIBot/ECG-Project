/*Made by the glorious Jesper*/

#ifndef CIRCULARARRAY_H_
#define CIRCULARARRAY_H_

/* A new datastructure, representing a circular array containing integers.
 * It is circular in the sense that when one inserts a new element,
 * the current index is set to the next position, and the element at the position is overwritten with the new element.
 * If it is at the end of the array, it loops back to the start.
 * The index of the elements in the array is measured according to the current element, which index is the currentIndex,
 * meaning it has position 0. It loops around the array, in the case that the index given is less than zero,
 * or greater than the size of the array.
 *
 * */

#include "peak.h"

union TPeakOrInt
{
	Peak peak;
	int integer;
};

typedef union TPeakOrInt PeakOrInt;

struct TCircularArray{
	int size; //The size of the circular array
	int allowedBitsMask;
	int startIndex; //The index of the current element in the array
	PeakOrInt* data; //The true array containing the data/different elements.
};

typedef struct TCircularArray CircularArray;

char initCircArray(CircularArray* const circArray, const int size, const int startIndex);
PeakOrInt getCircArrayPeakOrInt(const CircularArray* circArray, const int offset);
int getCircArrayValue(const CircularArray* circArray, const int offset);
int getArrayDataValue(const int data[], const int startIndex, const int arraySize, const int offset);
void moveCircArrayStartIndex(CircularArray* const circArray);
void insertCircArrayData(CircularArray* circArray, const int newData);
void freeCircArray(CircularArray* const circArray);

#endif /* CIRCULARARRAY_H_ */
