/*
 * rPeakFinder.h
 *
 *  Created on: 2016/09/10
 *      Author: jesper
 */

#ifndef INCLUDES_RPEAKFINDER_H_
#define INCLUDES_RPEAKFINDER_H_

#include "peakAvgCircularArray.h"

void initializeRPeakFinder();
char isRPeak(Peak newPeak);
PeakAvgCircularArray* getTrueRPeaksArray();
char getNewRPeaksFoundCount();
char isPulseUnstable();
unsigned short getPulse();
void freeRPeakFinder();
int getNoicePeakAverage();
#endif /* INCLUDES_RPEAKFINDER_H_ */
