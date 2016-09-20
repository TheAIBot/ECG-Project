/*
 * rPeakFinder.h
 *
 *  Created on: 2016/09/10
 *      Author: jesper
 */

#ifndef INCLUDES_RPEAKFINDER_H_
#define INCLUDES_RPEAKFINDER_H_

#include "peakCircularArray.h"

typedef struct TPeak Peak;
typedef struct TPeakCircularArray PeakCircularArray;

int isRPeak(int peakValue, int peakTime_0);
void recordNewProperRRPeak(int peakValue, int peakTime_0, int peakTime_7);
void searchBack();
int backwardCircularArray(int size, int currentIndex, int offset);
int forwardCircularArray(int size, int currentIndex, int offset);
int* getNewestTrueRRPeakTimes(int timesCount);
int* getNewestTrueRRPeakValues();
PeakCircularArray* getTrueRPeaks();

#endif /* INCLUDES_RPEAKFINDER_H_ */
