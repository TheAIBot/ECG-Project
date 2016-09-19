/*
 * lowPassFilter.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

#define GET_LOW_FILTER_VALUE_X_32 low_y_32

extern int low_y_32;

int getLowPassValue(int offset);
int lowPassFilter(int x, int x_6, int x_12);
void resetLowFilter(void);

#endif /* LOWPASSFILTER_H_ */
