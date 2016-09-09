/*
 * movingWindowFilter.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef MOVINGWINDOWFILTER_H_
#define MOVINGWINDOWFILTER_H_

#define N 30

short getMovingWindowValue(int offset);
short movingWindowFilter(short sqrDiffrence);

#endif /* MOVINGWINDOWFILTER_H_ */
