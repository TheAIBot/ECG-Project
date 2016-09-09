/*
 * filter.h
 *
 *  Created on: 9 Sep 2016
 *      Author: feynman
 */

#ifndef FILTER_H_
#define FILTER_H_

short getFilteredValue(int offset);
short filterData(short rawData, FILE* x_low, FILE* x_high, FILE* x_sqr, FILE* x_mwi);

#endif /* FILTER_H_ */
