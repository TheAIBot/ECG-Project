/*
 * peakSearcher.h
 *
 *  Created on: 2016/09/09
 *      Author: jesper
 */

#ifndef INCLUDES_PEAKSEARCHER_H_
#define INCLUDES_PEAKSEARCHER_H_


void testerPeak();
void searchPeak(int newDataPoint);
int isCurrentAPeak();
int nextPlaceCirkArray(int size, int current, int forward);
int formerPlaceCirkArray(int size, int current, int offset);
int nextIndexCirk(int size, int current);
int getPeakValue(int offsetCurrent);
int getPeakTime(int offsetCurrent);
int hasNewPeak();
void addRRTimeFromFormer();

#endif /* INCLUDES_PEAKSEARCHER_H_ */
