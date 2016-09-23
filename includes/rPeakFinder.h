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
char checkSearchback(Peak peakToCheck);
char passThreshold1(unsigned short intensity);
void recordNewProperRRPeak(int peakValue, int peakTime_0, int peakTime_7);
char searchBack();
int backwardCircularArray(int size, int currentIndex, int offset);
int forwardCircularArray(int size, int currentIndex, int offset);
PeakCircularArray* getTrueRPeaksArray();
Peak* getNewestPeak();
char getNewRPeaksFoundCount();
char isPulseUnstable();
void moveLastPeaksBackInArray();

#endif /* INCLUDES_RPEAKFINDER_H_ */
