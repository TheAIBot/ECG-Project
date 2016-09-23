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

char initCircArray(CircularArray* const circArray, const int size, const int startIndex);
int getCircArrayValue(const CircularArray* circArray, const int offset);
int getArrayDataValue(const int data[], const int startIndex, const int arraySize, const int offset);
void insertCircArrayData(CircularArray* circArray, const int newData);
void insertCircArrayDataAt(CircularArray* circArray, const int newData, const int offset);
void freeCircArray(CircularArray* const circArray);

#endif /* CIRCULARARRAY_H_ */
