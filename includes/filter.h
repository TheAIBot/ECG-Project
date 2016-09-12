/*
 * filter.h
 *
 *  Created on: 9 Sep 2016
 *      Author: feynman
 */

#ifndef FILTER_H_
#define FILTER_H_

short getFilteredValue(int offset);
short filterData(short rawData);
void resetFilteredBuffer(void);

#endif /* FILTER_H_ */
