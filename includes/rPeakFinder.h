/*
 * rPeakFinder.h
 *
 *  Created on: 2016/09/10
 *      Author: jesper
 */

#ifndef INCLUDES_RPEAKFINDER_H_
#define INCLUDES_RPEAKFINDER_H_

int isRPeak(int peakValue, int peakTime_0);
void recordNewProperRRPeak(int peakValue, int peakTime_0, int peakTime_7);
void searchBack();
int backwardCircularArray(int size, int currentIndex, int offset);
int forwardCircularArray(int size, int currentIndex, int offset);
int getNewestTrueRRPeakTime();
int getNewestTrueRRPeakValue();

#endif /* INCLUDES_RPEAKFINDER_H_ */
