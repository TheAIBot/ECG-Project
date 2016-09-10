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

void initCircularArray(struct CircularArray* circArray, int size, int startIndex);
int getDataValue(struct CircularArray* circArray, int offset);
int getArrayDataValue(int data[], int startIndex, int arraySize, int offset);
void insertData(struct CircularArray* circArray, int newData);
void moveArrayStartIndex(struct CircularArray* circArray);

#endif /* CIRCULARARRAY_H_ */
