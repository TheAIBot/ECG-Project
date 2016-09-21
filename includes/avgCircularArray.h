/*
 * abgCircularArray.h
 *
 *  Created on: 14 Sep 2016
 *      Author: feynman
 */

#ifndef INCLUDES_AVGCIRCULARARRAY_H_
#define INCLUDES_AVGCIRCULARARRAY_H_

#include "peak.h"

typedef struct TAvgCircularArray AvgCircularArray;

struct TAvgCircularArray{
	int size;
	int startIndex;
	int averageSum;
	int * data;
};

char initAvgCircArray(AvgCircularArray* avgCirc, int size, int startIndex, int defaultValue);
void setAvgCircValue(AvgCircularArray* avgCirc, const int offset, const int newValue);
int getAvgCircValue(const AvgCircularArray* avgCirc, const int offset);
void insertAvgCircData(AvgCircularArray* avgCirc, int newValue);
int getAvgCircAverageRR(const AvgCircularArray* avgCirc,int offset);
void resetAvgCirc(AvgCircularArray* avgCirc);
void freeAvgCirc(AvgCircularArray* avgCirc);


#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
