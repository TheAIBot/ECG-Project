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

struct TAvgCircularArray
{
	int size;
	int startIndex;
	int averageSum;
	int averageLength;
	Peak *data;
};


int getValue(const AvgCircularArray* avgCirc, const int offset);
static inline void moveAvgCircIndexForward(AvgCircularArray* avgCirc);
void insertAvgCircData(AvgCircularArray* avgCirc, const Peak newData);
int getAvgCircAverage(const AvgCircularArray* avgCirc);
void resetAvgCirc(AvgCircularArray* avgCirc);
void freeAvgCirc(AvgCircularArray* avgCirc);


#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
