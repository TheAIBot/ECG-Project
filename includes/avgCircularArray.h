/*
 * abgCircularArray.h
 *
 *  Created on: 14 Sep 2016
 *      Author: feynman
 */

#ifndef INCLUDES_AVGCIRCULARARRAY_H_
#define INCLUDES_AVGCIRCULARARRAY_H_

struct AvgCircularArray
{
	int size;
	int startIndex;
	int averageSum;
	int averageLength;
	int* data;
};

char initAvgCircArray(struct AvgCircularArray* avgCirc, int size, int startIndex, int avgLength);
int getAvgCircValue(struct AvgCircularArray* avgCirc, int offset);
void insertAvgCircData(struct AvgCircularArray* avgCirc, int newData);
int getAvgCircAverage(struct AvgCircularArray* avgCirc);
void moveAvgCircStartIndex(struct AvgCircularArray* circArray);
void freeAvgCirc(struct AvgCircularArray* avgCirc);

#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
