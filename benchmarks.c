#include <stdio.h>
#include "includes/inputManager.h"
#include "includes/benchmarks.h"
#include "includes/benchmarkTimer.h"
#include "includes/benchmarkCircularArray.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"
#include "includes/filter.h"

#define ECG_10800K_LENGTH 10800000 /* 10.800.000 */

int* loadECGArray()
{
	FILE* inputFile = startInputData("benchmark_files/ECG10800K.txt");

	if(inputFile == NULL)
	{
		exit(1);
	}

	//int inputArray[ECG_10800K_LENGTH];
	int* inputArray = malloc(ECG_10800K_LENGTH * sizeof(int));
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		inputArray[i] = getNextData(inputFile);
	}

	stopInputData(inputFile);

	return inputArray;
}

void printBenchmarkData(long result, long time, char* filterName)
{
	printf("%s filter %ld\n", filterName, result);
	BENCHMARK_PRINT_TIME(time);
}

void benchmarkLowPassFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		result += lowPassFilter(data[i], getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -6), getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -12));
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Low");
}

void benchmarkHighPassFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		result += highPassFilter(data[i], getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -16), getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -17), getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -32));
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "High");
}

void benchmarkDerivativeSquareFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		result += derivativeSquareFilter(data[i], getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -1), getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -3), getBenchmarkDataValue(data, i, ECG_10800K_LENGTH, -4));
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Deriv square");
}

void benchmarkMovingWindowFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		result += movingWindowFilter(data[i]);
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Moving window");
}

void benchmarhWholeFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		result += filterData(data[i], NULL, NULL, NULL, NULL);
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Whole filter");
}

void benchmarkAll()
{
	int* data = loadECGArray();

	benchmarkLowPassFilter(data);
	benchmarkHighPassFilter(data);
	benchmarkDerivativeSquareFilter(data);
	benchmarkMovingWindowFilter(data);
	benchmarhWholeFilter(data);

	free(data);
	exit(1);
}





