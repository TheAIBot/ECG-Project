/*
 * rPeakFinder.h
 *
 *  Created on: 2016/09/10
 *      Author: jesper
 */

#ifndef INCLUDES_RPEAKFINDER_H_
#define INCLUDES_RPEAKFINDER_H_
#include "peak.h"
#include "peakCircularArray.h"

typedef struct TPeak Peak;
typedef struct TPeakCircularArray PeakCircularArray;

void initializeRPeakFinder();
char isRPeak(Peak newPeak);
char checkSearchback(int indexToCheck);
char passThreshold1(Peak peak);
void recordNewProperRRPeak(int peakValue, int peakTime_0, int peakTime_7);
char searchBack();
int backwardCircularArray(int size, int currentIndex, int offset);
int forwardCircularArray(int size, int currentIndex, int offset);
PeakCircularArray* getTrueRPeaks();
Peak* getNewestPeak();
char isPulseUnstable();

#endif /* INCLUDES_RPEAKFINDER_H_ */
