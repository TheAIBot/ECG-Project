/*
 * peakCircularArray.h
 *
 *  Created on: 2016/09/20
 *      Author: jesper
 */

#ifndef INCLUDES_PEAKCIRCULARARRAY_H_
#define INCLUDES_PEAKCIRCULARARRAY_H_

#include "peak.h"

struct TPeakCircularArray{
	int size;
	int startIndex;
	Peak * data;
};

typedef struct TPeakCircularArray PeakCircularArray;

char initPeakCircArray(PeakCircularArray* const circArray, const int size, const int startIndex);
Peak getPeakCircArrayValue(const PeakCircularArray* circArray, const int offset);
void insertPeakCircArrayData(PeakCircularArray* const circArray, const Peak newData);
void freePeakCircArray(PeakCircularArray* const circArray);

#endif /* INCLUDES_PEAKCIRCULARARRAY_H_ */
