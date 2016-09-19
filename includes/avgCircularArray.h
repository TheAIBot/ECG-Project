/*
 * abgCircularArray.h
 *
 *  Created on: 14 Sep 2016
 *      Author: feynman
 */

#ifndef INCLUDES_AVGCIRCULARARRAY_H_
#define INCLUDES_AVGCIRCULARARRAY_H_

struct AvgCircularArray
{
	int size;
	int startIndex;
	int averageSum;
	int averageLength;
	int* data;
};

int getAvgCircValue(const struct AvgCircularArray* avgCirc, const int offset);
static inline void moveAvgCircStartIndex(struct AvgCircularArray* avgCirc);
void insertAvgCircData(struct AvgCircularArray* avgCirc, const int newData);
int getAvgCircAverage(const struct AvgCircularArray* avgCirc);
void resetAvgCirc(struct AvgCircularArray* avgCirc);
void freeAvgCirc(struct AvgCircularArray* avgCirc);



#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
