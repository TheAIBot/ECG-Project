#include <stdio.h>
#include <stdlib.h>
#include "includes/tests.h"
#include "includes/circularArray.h"
#include "includes/derSqrMwiFilter.h"
#include "includes/inputManager.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/filter.h"
#include "includes/rPeakFinder.h"
#include "includes/peakSearcher.h"

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

	struct CircularArray circArray;
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

	struct CircularArray circArray;
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

	struct CircularArray circArray;
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

		/*TODO. Temp test of peaksearcher. To be removed.*/
	}
	stopInputData(file);
	printf("Passed whole filter test\n");
	return 1;
}

char testSearchBackTripleSearchTest(){
	int dataSize = 702;
	int* testData = loadDataArray("verification_files/testSearchbackDouble.txt", dataSize);
	if (testData == NULL){
		free(testData);
		return 0;
	}
	for(int i = 0; i < dataSize; i++)
		{
			short filteredData = testData[i];
			searchPeak(filteredData);
			if (hasNewPeak() &&
				   isRPeak(getPeakValue(0), getPeakTime(0)))
			{
				int* trueRRPeaksRR = getTrueRPeaksArrayRR();
				int* trueRRPeaksVal = getTrueRPeaksArrayVal();
				int indexTrueRPeaks = getIndexTrueRPeaks();
				if (indexTrueRPeaks != 8+3)
					return 0;
				//The expected first 3 new peaks recorded, have value 2000,2000 and 5000:
				if (trueRRPeaksVal[indexTrueRPeaks - 3] != 2000 || trueRRPeaksVal[indexTrueRPeaks - 2] != 2000 ||
						 trueRRPeaksVal[indexTrueRPeaks - 1] != 5000){
					printf("%d,%d,%d \n",trueRRPeaksVal[indexTrueRPeaks - 3], trueRRPeaksVal[indexTrueRPeaks - 2], trueRRPeaksVal[indexTrueRPeaks - 1]);
					return 0;
				}
				//The expecte RR values are 174, (248 - 174) = 74 og (700 - 248)=452:
				if (trueRRPeaksRR[indexTrueRPeaks - 3] != 174 || trueRRPeaksRR[indexTrueRPeaks - 2] != 74 ||
						 trueRRPeaksRR[indexTrueRPeaks -1] != 452)
					return 0;

				//else:
				printf("Triple searchback test passed");
				return 1;
			}
		}
	free(testData);
	return 1;
}

char testRPeakSearcher(int* data)
{
	FILE* file = startInputData("verification_files/correct_Rpeak.txt");
	if(file == NULL)
	{
		return 0;
	}
	int* timesAndMeasurements = loadPeakData(file, TEST_DATA_R_PEAK_LENGTH);
	int* times = timesAndMeasurements;
	int* measurements = &timesAndMeasurements[TEST_DATA_R_PEAK_LENGTH];
	char* timeMeasurementTaken = calloc(TEST_DATA_R_PEAK_LENGTH, sizeof(char));

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		short filteredData = data[i];
		searchPeak(filteredData);
		if (hasNewPeak() &&
			   isRPeak(getPeakValue(0), getPeakTime(0)))
		{
		}
	}
	int* peakValues = getNewestTrueRRPeakValues();
	int* peakTimes = getNewestTrueRRPeakTimes(TEST_DATA_R_PEAK_LENGTH);
	for(int i = 0; i < TEST_DATA_R_PEAK_LENGTH; i++)
	{
		int peakValue = peakValues[i];
		int peakTime = peakTimes[i];
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


	if (!testSearchBackTripleSearchTest()){
		printf("Failed triple searchback test");
		return;
	}

	printf("All tests finished successfully\n");
}






