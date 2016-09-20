/*
 * peakCircularArray.h
 *
 *  Created on: 2016/09/20
 *      Author: jesper
 */

#ifndef INCLUDES_PEAKCIRCULARARRAY_H_
#define INCLUDES_PEAKCIRCULARARRAY_H_

#include "peak.h"

typedef struct TPeakCircularArray PeakCircularArray;

struct TPeakCircularArray
{
	int size;
	int startIndex;
	Peak** data;
};

char initPeakCircArray(PeakCircularArray* circArray, int size, int startIndex);
Peak* getPeakCircArrayValue(PeakCircularArray* circArray, int offset);
Peak* getPeakCircArrayDataValue(Peak data[], int startIndex, int arraySize, int offset);
void insertPeakCircArrayData(PeakCircularArray* circArray, Peak* newData);
void freePeakCircArray(PeakCircularArray* circArray);

#endif /* INCLUDES_PEAKCIRCULARARRAY_H_ */
