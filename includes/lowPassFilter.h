/*
 * lowPassFilter.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

short getLowPassValue(int offset);
short lowPassFilter(short x, short x_6, short x_12);

#endif /* LOWPASSFILTER_H_ */
