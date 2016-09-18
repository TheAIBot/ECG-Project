/*
 * derivativeSquareFilter.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef DERIVATIVESQUAREFILTER_H_
#define DERIVATIVESQUAREFILTER_H_

short* getSquareArray();
short getSqrValue(int offset);
unsigned short derivativeSquareMovingWindowFilter(int x, int x_1, int x_3, int x_4);
void resetSqrBuffer(void);

#endif /* DERIVATIVESQUAREFILTER_H_ */
