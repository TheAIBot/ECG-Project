/*
 * rawData.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef RAWDATA_H_
#define RAWDATA_H_

#define GET_RAW_VALUE_X_12 raw_y_12

extern short raw_y_12;

short getRawDataValue(const int offset);
void insertRawData(const short data);
void resetRawBuffer(void);

#endif /* RAWDATA_H_ */
