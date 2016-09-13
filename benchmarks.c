#include <stdio.h>
#include <stdlib.h>
#include "includes/inputManager.h"
#include "includes/benchmarks.h"
#include "includes/benchmarkTimer.h"
#include "includes/circularArray.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"
#include "includes/filter.h"

#define ECG_10800K_LENGTH 10800000 /* 10.800.000 */
static long result = 0;

void printBenchmarkData(long time, char* filterName)
{
	printf("%s filter %ld\n", filterName, result);
	BENCHMARK_PRINT_TIME(time);
}

void benchmarkXTimes(int (*function) (int*), int times, int* data, char* functionName)
{
	int i;
	int benchmarkTime = 0;
	for(i = 0; i < times; i++)
	{
		benchmarkTime += (*function)(data);
	}
	printBenchmarkData(benchmarkTime / times, functionName);
}

int benchmarkLowPassFilter(int* data)
{
	long startTime = BENCHMARK_START;
	result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += lowPassFilter(data[i], getArrayDataValue(data, i, ECG_10800K_LENGTH, -6), getArrayDataValue(data, i, ECG_10800K_LENGTH, -12));
	}
	return BENCHMARK_TIME(startTime);
}

int benchmarkHighPassFilter(int* data)
{
	long startTime = BENCHMARK_START;
	result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += highPassFilter(data[i], getArrayDataValue(data, i, ECG_10800K_LENGTH, -16), getArrayDataValue(data, i, ECG_10800K_LENGTH, -32));
	}
	return BENCHMARK_TIME(startTime);
}

int benchmarkDerivativeSquareFilter(int* data)
{
	long startTime = BENCHMARK_START;
	result = 0;
	short y_1 = 0;
	short y_1_1 = 0;
	short y_2 = 0;
	short y_3 = 0;
	short y_4 = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		y_4 = y_3;
		y_3 = y_2;
		y_2 = y_1;
		y_1 = y_1_1;
		y_1_1 = data[i];
		result += derivativeSquareFilter(y_1_1, y_1, y_3, y_4);
	}
	return BENCHMARK_TIME(startTime);
}

int benchmarkMovingWindowFilter(int* data)
{
	long startTime = BENCHMARK_START;
	result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += movingWindowFilter(data[i]);
	}
	return BENCHMARK_TIME(startTime);
}

int benchmarhWholeFilter(int* data)
{
	long startTime = BENCHMARK_START;
	result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += filterData(data[i]);
	}
	return BENCHMARK_TIME(startTime);
}

void runBenchmarks()
{
	int* data = loadDataArray("benchmark_files/ECG10800K.txt", ECG_10800K_LENGTH);

	if(data == NULL)
	{
		return;
	}
	benchmarkXTimes(&benchmarkLowPassFilter, 20, data, "low");
	benchmarkXTimes(&benchmarkHighPassFilter, 20, data, "high");
	benchmarkXTimes(&benchmarkDerivativeSquareFilter, 20, data, "derivative square");
	benchmarkXTimes(&benchmarkMovingWindowFilter, 20, data, "moving window");
	benchmarkXTimes(&benchmarhWholeFilter, 20, data, "whole");

	free(data);
}





