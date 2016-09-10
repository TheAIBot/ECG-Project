#include <stdio.h>
#include <stdlib.h>
#include "includes/tests.h"
#include "includes/circularArray.h"
#include "includes/inputManager.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"
#include "includes/filter.h"

#define TEST_DATA_LENGTH 10000 /* 10.000 */

char checkFilter(int filterOutput, FILE* file, char* filterName)
{
	if(file != NULL)
	{
		int expectedOutput = getNextData(file);
		if(filterOutput != expectedOutput)
		{
			fprintf(stderr, "wrong output from %s filter: Given %hd != %hd Expected\n", filterName, filterOutput, expectedOutput);
			return 1;
		}
	}
	return 0;
}

void flushFilterBuffers()
{
	resetRawBuffer();
	resetLowBuffer();
	resetHighBuffer();
	resetSqrBuffer();
	resetMovingWindowBuffer();
	resetFilteredBuffer();
}

char testLowPassFilter(int* data)
{
	resetLowBuffer();
	FILE* file = startInputData("verification_files/x_low.txt");
	if(file == NULL)
	{
		return 0;
	}

	struct CircularArray circArray;
	initCircularArray(&circArray, 13, 0);

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertData(&circArray, data[i]);

		int dataLowFiltered = lowPassFilter(data[i], getDataValue(&circArray, -6), getDataValue(&circArray, -12));
		if(checkFilter(dataLowFiltered, file, "low"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	free(circArray.data);
	printf("Passed low pass filter test\n");
	return 1;
}

char testHighPassFilter(int* data)
{
	resetHighBuffer();
	FILE* file = startInputData("verification_files/x_high.txt");
	if(file == NULL)
	{
		return 0;
	}

	struct CircularArray circArray;
	initCircularArray(&circArray, 33, 0);

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertData(&circArray, data[i]);

		int dataLowFiltered = highPassFilter(data[i], getDataValue(&circArray, -16), getDataValue(&circArray, -17), getDataValue(&circArray, -32));
		if(checkFilter(dataLowFiltered, file, "high"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	free(circArray.data);
	printf("Passed high pass filter test\n");
	return 1;
}

char testDerivSqrFilter(int* data)
{
	resetSqrBuffer();
	FILE* file = startInputData("verification_files/x_sqr.txt");
	if(file == NULL)
	{
		return 0;
	}

	struct CircularArray circArray;
	initCircularArray(&circArray, 5, 0);

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertData(&circArray, data[i]);

		derivativeSquareFilter(data[i], getDataValue(&circArray, -1), getDataValue(&circArray, -3), getDataValue(&circArray, -4));
		if(checkFilter(getSqrValue(0), file, "derivative square"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	free(circArray.data);
	printf("Passed derivative square filter test\n");
	return 1;
}

char testMovingwindowFilter(int* data)
{
	resetMovingWindowBuffer();
	FILE* file = startInputData("verification_files/x_mwi_div_after.txt");
	if(file == NULL)
	{
		return 0;
	}

	struct CircularArray circArray;
	initCircularArray(&circArray, 31, 0);

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertData(&circArray, data[i]);

		int dataLowFiltered = movingWindowFilter(data[i] - getDataValue(&circArray, -N));
		if(checkFilter(dataLowFiltered, file, "moving window"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	free(circArray.data);
	printf("Passed moving window filter test\n");
	return 1;
}

char testWholeFilter(int* data)
{
	flushFilterBuffers();
	FILE* file = startInputData("verification_files/x_mwi_div_after.txt");
	if(file == NULL)
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{

		int dataLowFiltered = filterData(data[i]);
		if(checkFilter(dataLowFiltered, file, "whole"))
		{
			stopInputData(file);
			return 0;
		}
	}
	stopInputData(file);
	printf("Passed whole filter test\n");
	return 1;
}

void testAll()
{
	int* ecgData = loadDataArray("ECG.txt", TEST_DATA_LENGTH);
	if(ecgData == NULL ||
	   !testLowPassFilter(ecgData))
	{
		return;
	}
	/*ecgData is freed at the bottom*/

	int* lowData = loadDataArray("verification_files/x_low.txt", TEST_DATA_LENGTH);
	if(lowData == NULL ||
	   !testHighPassFilter(lowData))
	{
		return;
	}
	free(lowData);

	int* highData = loadDataArray("verification_files/x_high.txt", TEST_DATA_LENGTH);
	if(highData == NULL ||
	   !testDerivSqrFilter(highData))
	{
		return;
	}
	free(highData);

	int* sqrData = loadDataArray("verification_files/x_sqr.txt", TEST_DATA_LENGTH);
	if(sqrData == NULL ||
	   !testMovingwindowFilter(sqrData))
	{
		return;
	}
	free(sqrData);

	if(ecgData == NULL ||
	   !testWholeFilter(ecgData))
	{
		return;
	}
	free(ecgData);


	printf("All tests finished successfully");
}






