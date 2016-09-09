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

short filteredData[FILTERED_DATA_SIZE];
short arrayStartIndex6 = 0;

short getFilteredValue(int offset)
{
	short correctIndex = arrayStartIndex6 + offset;
	if(correctIndex < 0)
	{
		correctIndex += FILTERED_DATA_SIZE;
	}
	return filteredData[correctIndex];
}

void moveArrayStartIndex6()
{
	arrayStartIndex6++;
	if(arrayStartIndex6 == FILTERED_DATA_SIZE)
	{
		arrayStartIndex6 = 0;
	}
}

char checkFilter(int filterOutput, FILE* file, char* filterName)
{
	if(file != NULL)
	{
		int expectedOutput = getNextData(file);
		if(filterOutput != expectedOutput)
		{
			printf("wrong output from %s pass filter: %d != %d\n", filterName, filterOutput, expectedOutput);
			return 1;
		}
	}
	return 0;
}

short filterData(short rawData, FILE* x_low, FILE* x_high, FILE* x_sqr, FILE* x_mwi)
{
	moveArrayStartIndex6();

	insertRawData(rawData);

	int dataLowFiltered = lowPassFilter(rawData, getRawDataValue(-6), getRawDataValue(-12));
	if(checkFilter(dataLowFiltered, x_low, "low"))
	{
		exit(1);
	}

	short dataHighFiltered = highPassFilter(dataLowFiltered, getLowPassValue(-16), getLowPassValue(-17), getLowPassValue(-32));
	if(checkFilter(dataHighFiltered, x_high, "high"))
	{
		exit(1);
	}

	short dataDerSqrFiltered = derivativeSquareFilter(dataHighFiltered, getHighPassValue(-1), getHighPassValue(-3), getHighPassValue(-4));
	if(checkFilter(dataDerSqrFiltered, x_sqr, "dersqr"))
	{
		exit(1);
	}

	short dataMovingWindowFilter = movingWindowFilter(getSquareArray());
	if(checkFilter(dataMovingWindowFilter, x_mwi, "moving window"))
	{
		exit(1);
	}

	filteredData[arrayStartIndex6] = dataMovingWindowFilter;
	return dataMovingWindowFilter;
}





