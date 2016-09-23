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
PeakCircularArray* getTrueRPeaksArray();
char getNewRPeaksFoundCount();
char isPulseUnstable();

#endif /* INCLUDES_RPEAKFINDER_H_ */
