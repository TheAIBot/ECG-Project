#include <stdlib.h>
#include <stdio.h>
#include "includes/peakCircularArray.h"

char initPeakCircArray(PeakCircularArray* const circArray, const int size, const int startIndex){
	return initCircArray(circArray, size, startIndex);
}

Peak getPeakCircArrayValue(const PeakCircularArray* circArray, const int offset){
	return getCircArrayPeakOrInt(circArray, offset).peak;
}

void insertPeakCircArrayData(PeakCircularArray* const circArray, const Peak newData){
	insertCircArrayPeakOrInt(circArray, (PeakOrInt)newData);
}

void freePeakCircArray(PeakCircularArray* const circArray){
	freeCircArray(circArray);
}








