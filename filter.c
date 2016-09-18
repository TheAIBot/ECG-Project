#include <stdio.h>
#include <stdlib.h>
#include "includes/filter.h"

#include "includes/derSqrMwiFilter.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/inputManager.h"

#define FILTERED_DATA_SIZE 5

static short filteredData[FILTERED_DATA_SIZE];
static short arrayStartIndex = 0;

short getFilteredValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += FILTERED_DATA_SIZE;
	}
	return filteredData[correctIndex];
}

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == FILTERED_DATA_SIZE)
	{
		arrayStartIndex = 0;
	}
}

short filterData(short rawData)
{
	moveArrayStartIndex();
	insertRawData(rawData);

	/*low pass filter*/
	int dataLowFiltered = lowPassFilter(rawData, getRawDataValue(-6), GET_LAST_RAW_VALUE);

	/*high pass filter*/
	short dataHighFiltered = highPassFilter(dataLowFiltered, getLowPassValue(-16), GET_LAST_LOW_FILTER_VALUE);

	/*derivative, square and moving window filter*/
	return derivativeSquareMovingWindowFilter(dataHighFiltered, GET_HIGH_PASS_VALUE(X_1_INDEX), GET_HIGH_PASS_VALUE(X_3_INDEX), GET_HIGH_PASS_VALUE(X_4_INDEX));
}

void resetFilteredBuffer()
{
	memset(filteredData, 0, FILTERED_DATA_SIZE * sizeof(short));
}





