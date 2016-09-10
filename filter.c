#include <stdio.h>
#include <stdlib.h>
#include "includes/filter.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"
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
	int dataLowFiltered = lowPassFilter(rawData, getRawDataValue(-6), getRawDataValue(-12));

	/*high pass filter*/
	short dataHighFiltered = highPassFilter(dataLowFiltered, getLowPassValue(-16), getLowPassValue(-17), getLowPassValue(-32));

	/*derivative and square filter*/
	short dataDerSqrFiltered = derivativeSquareFilter(dataHighFiltered, getHighPassValue(-1), getHighPassValue(-3), getHighPassValue(-4));

	/*moving window filter*/
	short dataMovingWindowFilter = movingWindowFilter(dataDerSqrFiltered);

	filteredData[arrayStartIndex] = dataMovingWindowFilter;
	return dataMovingWindowFilter;
}

void resetFilteredBuffer()
{
	memset(filteredData, 0, FILTERED_DATA_SIZE * sizeof(short));
}





