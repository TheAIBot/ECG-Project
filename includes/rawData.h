/*
 * rawData.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef RAWDATA_H_
#define RAWDATA_H_

#define RAW_DATA_SIZE 13

short getRawDataValue(int offset);
void insertRawData(short data);
void resetRawBuffer(void);

#endif /* RAWDATA_H_ */
