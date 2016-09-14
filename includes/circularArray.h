/*
 * benchmarkCircularArray.h
 *
 *  Created on: 9 Sep 2016
 *      Author: feynman
 */

#ifndef CIRCULARARRAY_H_
#define CIRCULARARRAY_H_

struct CircularArray
{
	int size;
	int startIndex;
	int* data;
};

char initCircArray(struct CircularArray* circArray, int size, int startIndex);
int getCircArrayValue(struct CircularArray* circArray, int offset);
int getArrayDataValue(int data[], int startIndex, int arraySize, int offset);
void insertCircArrayData(struct CircularArray* circArray, int newData);

#endif /* CIRCULARARRAY_H_ */
