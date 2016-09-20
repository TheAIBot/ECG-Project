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
	Peak** data;
};

char initAvgCircArray(struct TAvgCircularArray* avgCirc, int size, int startIndex, int averageLength, Peak defaultPeak);
void setAvgCircValue(struct TAvgCircularArray* avgCirc, const int offset, const Peak* peak);
Peak* getAvgCircValue(const struct TAvgCircularArray* avgCirc, const int offset);
//static void moveAvgCircIndexForward(AvgCircularArray* avgCirc);
void insertAvgCircData(AvgCircularArray* avgCirc, Peak* newPeak);
int getAvgCircAverageRR(const struct TAvgCircularArray* avgCirc);
void resetAvgCirc(struct TAvgCircularArray* avgCirc);
void freeAvgCirc(struct TAvgCircularArray* avgCirc);


#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
