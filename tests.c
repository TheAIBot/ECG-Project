#include <stdio.h>
#include <stdlib.h>
#include "includes/tests.h"
#include "includes/circularArray.h"
#include "includes/derSqrMwiFilter.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/filter.h"
#include "includes/rPeakFinder.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/sensor.h"
#include "includes/peakCircularArray.h"


typedef struct TPeakCircularArray PeakCircularArray;


#define TEST_DATA_LENGTH 10000 /* 10.000 */
#define TEST_DATA_R_PEAK_LENGTH 31
#define ALLOWED_TIME_DEVIANTION 10
#define ALLOWED_MEASUREMENT_DEVIATION 300

char isFilterCorrect(int filterOutput, FILE* file, char* filterName)
{
	if(file != NULL)
	{
		int expectedOutput = getNextData(file);
		if(filterOutput != expectedOutput)
		{
			fprintf(stderr, "wrong output from %s filter: Given %hd != %hd Expected\n", filterName, filterOutput, expectedOutput);
			return 0;
		}
	}
	return 1;
}

void flushFilterBuffers()
{
	resetRawBuffer();
	resetLowFilter();
	resetHighFilter();
	resetDerSqrMwiFilter();
}

char testLowPassFilter(int* data)
{
	resetLowFilter();
	FILE* file = startInputData("verification_files/x_low.txt");
	if(file == NULL)
	{
		return 0;
	}

	CircularArray circArray;
	if(!initCircArray(&circArray, 13, 0))
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertCircArrayData(&circArray, data[i]);

		int dataLowFiltered = lowPassFilter(data[i], getCircArrayValue(&circArray, -6), getCircArrayValue(&circArray, -12));
		if(!isFilterCorrect(dataLowFiltered, file, "low"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	freeCircArray(&circArray);
	printf("Passed low pass filter test\n");
	return 1;
}

char testHighPassFilter(int* data)
{
	resetHighFilter();
	FILE* file = startInputData("verification_files/x_high.txt");
	if(file == NULL)
	{
		return 0;
	}

	CircularArray circArray;
	if(!initCircArray(&circArray, 33, 0))
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertCircArrayData(&circArray, data[i]);

		int dataLowFiltered = highPassFilter(data[i], getCircArrayValue(&circArray, -16), getCircArrayValue(&circArray, -32));
		if(!isFilterCorrect(dataLowFiltered, file, "high"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	freeCircArray(&circArray);
	printf("Passed high pass filter test\n");
	return 1;
}

char testDerivSqrFilter(int* data)
{
	resetDerSqrMwiFilter();
	FILE* file = startInputData("verification_files/x_mwi_div_after.txt");
	if(file == NULL)
	{
		return 0;
	}

	CircularArray circArray;
	if(!initCircArray(&circArray, 5, 0))
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertCircArrayData(&circArray, data[i]);

		short mwiFiltered = derivativeSquareMovingWindowFilter(data[i], getCircArrayValue(&circArray, -1), getCircArrayValue(&circArray, -3), getCircArrayValue(&circArray, -4));
		if(!isFilterCorrect(mwiFiltered, file, "derivative square moving window"))
		{
			stopInputData(file);
			free(circArray.data);
			return 0;
		}
	}
	stopInputData(file);
	freeCircArray(&circArray);
	printf("Passed derivative square moving window filter test\n");
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
		if(!isFilterCorrect(dataLowFiltered, file, "whole"))
		{
			stopInputData(file);
			return 0;
		}

	}
	stopInputData(file);
	printf("Passed whole filter test\n");
	return 1;
}

char testRPeakSearcher(int* data)
{
	FILE* file = startInputData("verification_files/correct_Rpeak.txt");
	if(file == NULL)
	{
		return 0;
	}
	unsigned short* timesAndMeasurements = loadPeakData(file, TEST_DATA_R_PEAK_LENGTH);
	unsigned short* times = timesAndMeasurements;
	unsigned short* measurements = &timesAndMeasurements[TEST_DATA_R_PEAK_LENGTH];
	char* timeMeasurementTaken = calloc(TEST_DATA_R_PEAK_LENGTH, sizeof(char));
/*
	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		if(foundPeak(data[i]))
		{
			Peak newPeak = getNewPeak();
			if (isRPeak(newPeak)){
				printf("%d\n",i);
			}
		}
	}
	printf("fisk\n");
	*/
	//TODO fix test.
	/*
	unsigned short* peakValues = getPeakValues();
	unsigned short* peakTimes = getPeakTimes(TEST_DATA_R_PEAK_LENGTH);
	printf("sdfsd\n");
	for(int i = 0; i < TEST_DATA_R_PEAK_LENGTH; i++)
	{
		unsigned short peakValue = peakValues[i];
		unsigned short peakTime = peakTimes[i];
		char isCorrect = 0;

		for(int y = 0; y < TEST_DATA_R_PEAK_LENGTH; y++)
		{
			if(timeMeasurementTaken[y] == 0 &&
			   times[y] + ALLOWED_TIME_DEVIANTION >= peakTime &&
			   times[y] - ALLOWED_TIME_DEVIANTION <= peakTime &&
			   measurements[y] + ALLOWED_MEASUREMENT_DEVIATION >= peakValue &&
			   measurements[y] - ALLOWED_MEASUREMENT_DEVIATION <= peakValue)
			{
				timeMeasurementTaken[y] = 1;
				isCorrect = 1;
				break;
			}
		}
		if(!isCorrect)
		{
			printf("Failed to find matching peak for time: %d, measurement: %d\n", peakTime, peakValue);
			free(timesAndMeasurements);
			free(timeMeasurementTaken);
			return 0;
		}
	}
	char foundAll = 1;
	for(int i = 0; i < TEST_DATA_R_PEAK_LENGTH; i++)
	{
		if(timeMeasurementTaken[i] == 0)
		{
			foundAll = 0;
			printf("Couldn't find a match for time: %d, value: %d\n", times[i], measurements[i]);
			break;
		}
	}
	if(!foundAll)
	{
		printf("Failed to find all peaks\n");
	}
	else
	{
		printf("Passed r peak searcher test\n");
	}
	free(timesAndMeasurements);
	free(timeMeasurementTaken);
	*/
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
		free(lowData);
		return;
	}
	free(lowData);

	int* highData = loadDataArray("verification_files/x_high.txt", TEST_DATA_LENGTH);
		if(highData == NULL ||
		   !testDerivSqrFilter(highData))
		{
			free(highData);
			return;
		}
	free(highData);

	if(ecgData == NULL ||
	   !testWholeFilter(ecgData))
	{
		free(ecgData);
		return;
	}
	free(ecgData);


	int* mwi_after = loadDataArray("verification_files/x_mwi_div_after.txt", TEST_DATA_LENGTH);
	if(mwi_after == NULL ||
	   !testRPeakSearcher(mwi_after))
	{
		free(mwi_after);
		return;
	}
	free(mwi_after);

	printf("All tests finished successfully\n");
}






