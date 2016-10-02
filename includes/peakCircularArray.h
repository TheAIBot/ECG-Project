/*
 * peakCircularArray.h
 *
 *  Created on: 2016/09/20
 *      Author: jesper
 */

#ifndef INCLUDES_PEAKCIRCULARARRAY_H_
#define INCLUDES_PEAKCIRCULARARRAY_H_

//#include "peak.h"
#include "circularArray.h"

/* A new datastructure, representing a circular array containing Peaks.
 * It is circular in the sense that when one inserts a new element,
 * the current index is set to the next position, and the element at the position is overwritten with the new element.
 * If it is at the end of the array, it loops back to the start.
 * The index of the elements in the array is measured according to the current element, which index is the currentIndex,
 * meaning it has position 0. It loops around the array, in the case that the index given is less than zero,
 * or greater than the size of the array.
 *
 * */


typedef struct TCircularArray PeakCircularArray;

char initPeakCircArray(PeakCircularArray* const circArray, const int size, const int startIndex);
Peak getPeakCircArrayValue(const PeakCircularArray* circArray, const int offset);
void insertPeakCircArrayData(PeakCircularArray* const circArray, const Peak newData);
void freePeakCircArray(PeakCircularArray* const circArray);

#endif /* INCLUDES_PEAKCIRCULARARRAY_H_ */
