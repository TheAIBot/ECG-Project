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

void printBenchmarkData(long result, long time, char* filterName)
{
	printf("%s filter %ld\n", filterName, result);
	BENCHMARK_PRINT_TIME(time);
}

void benchmarkLowPassFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += lowPassFilter(data[i], getArrayDataValue(data, i, ECG_10800K_LENGTH, -6), getArrayDataValue(data, i, ECG_10800K_LENGTH, -12));
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Low");
}

void benchmarkHighPassFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += highPassFilter(data[i], getArrayDataValue(data, i, ECG_10800K_LENGTH, -16), getArrayDataValue(data, i, ECG_10800K_LENGTH, -32));
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "High");
}

void benchmarkDerivativeSquareFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
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
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Deriv square");
}

void benchmarkMovingWindowFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += movingWindowFilter(data[i]);
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Moving window");
}

void benchmarhWholeFilter(int* data)
{
	long startTime = BENCHMARK_START;
	long result = 0;
	int i = 0;
	for(; i < ECG_10800K_LENGTH; i++)
	{
		result += filterData(data[i]);
	}
	printBenchmarkData(result, BENCHMARK_TIME(startTime), "Whole");
}

void runBenchmarks()
{
	int* data = loadDataArray("benchmark_files/ECG10800K.txt", ECG_10800K_LENGTH);

	if(data == NULL)
	{
		return;
	}

	benchmarkLowPassFilter(data);
	benchmarkHighPassFilter(data);
	benchmarkDerivativeSquareFilter(data);
	benchmarkMovingWindowFilter(data);
	/*for(int i = 0; i < 100; i++)*/
	/*{*/
		benchmarhWholeFilter(data);
	/*}*/

	free(data);
}





