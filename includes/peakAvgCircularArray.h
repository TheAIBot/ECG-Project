#ifndef INCLUDES_AVGPEAKCIRCULARARRAY_H_
#define INCLUDES_AVGPEAKCIRCULARARRAY_H_

#include "peak.h"
#include "avgCircularArray.h"

typedef struct TAvgCircularArray PeakAvgCircularArray;

char initPeakAvgCircArray(PeakAvgCircularArray* const peakAvgCirc, const int size, const int startIndex, const Peak defaultPeak);
Peak getPeakAvgCircValue(const PeakAvgCircularArray* peakAvgCirc, const int offset);
void insertPeakAvgCircData(PeakAvgCircularArray* const peakAvgCirc, const Peak newPeak);
int getPeakAvgCircAverageValue(const PeakAvgCircularArray* peakAvgCirc);
void resetPeakAvgCirc(PeakAvgCircularArray*  const peakAvgCirc);
void freePeakAvgCirc(PeakAvgCircularArray* const peakAvgCirc);


#endif /*INCLUDES_AVGPEAKCIRCULARARRAY_H_ */
