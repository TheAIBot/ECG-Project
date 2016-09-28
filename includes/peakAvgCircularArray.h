#ifndef INCLUDES_AVGPEAKCIRCULARARRAY_H_
#define INCLUDES_AVGPEAKCIRCULARARRAY_H_

#include "peak.h"



/* A new datastructure, representing a circular array containing peaks,
 * that keeps a record of the average RR value of the element in it. It is circular in the sense that when one inserts a new element,
 * the current index is set to the next position, and the element at the position is overwritten with the new element.
 * If it is at the end of the array, it loops back to the start.
 * The index of the elements in the array is measured according to the current element, which index is the currentIndex,
 * meaning it has position 0. It loops around the array, in the case that the index given is less than zero,
 * or greater than the size of the array.
 *
 * */
struct TPeakAvgCircularArray {
	int size;
	int startIndex; //The current index.
	int averageSum; //The sum of the RR value of all the elements in the array.
	Peak * data; //The actual array containing the elements.
};

typedef struct TPeakAvgCircularArray PeakAvgCircularArray;

char initPeakAvgCircArray(PeakAvgCircularArray* const peakAvgCirc, const int size, const int startIndex, const Peak defaultPeak);
Peak getPeakAvgCircValue(const PeakAvgCircularArray* peakAvgCirc, const int offset);
void insertPeakAvgCircData(PeakAvgCircularArray* const peakAvgCirc, const Peak newPeak);
int getPeakAvgCircAverageValue(const PeakAvgCircularArray* peakAvgCirc);
void resetPeakAvgCirc(PeakAvgCircularArray*  const peakAvgCirc);
void freePeakAvgCirc(PeakAvgCircularArray* const peakAvgCirc);


#endif /*INCLUDES_AVGPEAKCIRCULARARRAY_H_ */
