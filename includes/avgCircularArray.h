#ifndef INCLUDES_AVGCIRCULARARRAY_H_
#define INCLUDES_AVGCIRCULARARRAY_H_

#include "peak.h"
#include "circularArray.h"

/* A new datastructure, representing a circular array containing integers,
 * that keeps a record of the average of the element in it. It is circular in the sense that when one inserts a new element,
 * the current index is set to the next position, and the element at the position is overwritten with the new element.
 * If it is at the end of the array, it loops back to the start.
 * The index of the elements in the array is measured according to the current element, which index is the currentIndex,
 * meaning it has position 0. It loops around the array, in the case that the index given is less than zero,
 * or greater than the size of the array.
 *
 * */
struct TAvgCircularArray {
	CircularArray circArray;
	int averageSum; //The sum of the value of all the elements in the array.
};

typedef struct TAvgCircularArray AvgCircularArray;

char initAvgCircArray(AvgCircularArray* const avgCirc, const int size, const int startIndex, const int defaultValue);
int getAvgCircValue(const AvgCircularArray* avgCirc, const int offset);
void insertAvgCircData(AvgCircularArray* const avgCirc, const int newValue);
int getAvgCircAverageValue(const AvgCircularArray* avgCirc);
void resetAvgCirc(AvgCircularArray*  const avgCirc);
void freeAvgCirc(AvgCircularArray* const avgCirc);


#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
