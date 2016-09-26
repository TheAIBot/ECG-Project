#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/filter.h"
#include "includes/derSqrMwiFilter.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/sensor.h"

//This method filters the rawData given with previous given values
//rawData is the data to filter
//returns the low pass, high pass derivative, squared and moving window filtered result.
unsigned short filterData(const short rawData)
{
	//need to store the last 12 rawData values which is done with this cirkular array
	insertRawData(rawData);

	/*low pass filter*/
	int dataLowFiltered = lowPassFilter(rawData, getRawDataValue(-6), getRawDataValue(-12));

	/*high pass filter*/
	short dataHighFiltered = highPassFilter(dataLowFiltered, getLowPassValue(-16), GET_LOW_FILTER_VALUE_X_32);

	/*derivative, square and moving window filter*/
	return derivativeSquareMovingWindowFilter(dataHighFiltered, GET_HIGH_PASS_VALUE(X_1_INDEX), GET_HIGH_PASS_VALUE(X_3_INDEX), GET_HIGH_PASS_VALUE(X_4_INDEX));
}





