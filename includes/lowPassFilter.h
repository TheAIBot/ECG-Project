/*
 * lowPassFilter.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

#define GET_LAST_LOW_FILTER_VALUE y_32

extern int y_32;

int getLowPassValue(int offset);
int lowPassFilter(int x, int x_6, int x_12);
void resetLowBuffer(void);

#endif /* LOWPASSFILTER_H_ */
