/*
 * rawData.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */

#ifndef RAWDATA_H_
#define RAWDATA_H_

short getRawDataValue(const int offset);
void insertRawData(const short data);
void resetRawBuffer(void);

#endif /* RAWDATA_H_ */
