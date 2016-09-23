#ifndef INCLUDES_AVGCIRCULARARRAY_H_
#define INCLUDES_AVGCIRCULARARRAY_H_

#include "peak.h"

struct TAvgCircularArray {
	int size;
	int startIndex;
	int averageSum;
	int * data;
};

typedef struct TAvgCircularArray AvgCircularArray;

char initAvgCircArray(AvgCircularArray* const avgCirc, const int size, const int startIndex, const int defaultValue);
int getAvgCircValue(const AvgCircularArray* avgCirc, const int offset);
void insertAvgCircData(AvgCircularArray* const avgCirc, const int newValue);
int getAvgCircAverageValue(const AvgCircularArray* avgCirc);
void resetAvgCirc(AvgCircularArray*  const avgCirc);
void freeAvgCirc(AvgCircularArray* const avgCirc);


#endif /* INCLUDES_AVGCIRCULARARRAY_H_ */
