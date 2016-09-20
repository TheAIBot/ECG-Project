/*
 * benchmarkCircularArray.h
 *
 *  Created on: 9 Sep 2016
 *      Author: feynman
 */

#ifndef CIRCULARARRAY_H_
#define CIRCULARARRAY_H_

struct TCircularArray
{
	int size;
	int startIndex;
	int* data;
};

typedef struct TCircularArray CircularArray;

char initCircArray(CircularArray* circArray, int size, int startIndex);
int getCircArrayValue(CircularArray* circArray, int offset);
int getArrayDataValue(int data[], int startIndex, int arraySize, int offset);
void insertCircArrayData(CircularArray* circArray, int newData);
void freeCircArray(CircularArray* circArray);

#endif /* CIRCULARARRAY_H_ */
