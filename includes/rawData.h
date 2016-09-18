/*
 * rawData.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef RAWDATA_H_
#define RAWDATA_H_

#define GET_LAST_RAW_VALUE y_12

extern short y_12;

short getRawDataValue(int offset);
void insertRawData(short data);
void resetRawBuffer(void);

#endif /* RAWDATA_H_ */
