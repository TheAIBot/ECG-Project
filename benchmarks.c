#include <stdio.h>
#include <stdlib.h>
#include "includes/benchmarks.h"
#include "includes/benchmarkTimer.h"
#include "includes/circularArray.h"
#include "includes/derSqrMwiFilter.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/filter.h"
#include "includes/peakSearcher.h"
#include "includes/rPeakFinder.h"
#include "includes/sensor.h"

#define ECG_10800K_LENGTH 10800000 /* 10.800.000 */
static long result = 0;

static void printBenchmarkData(long time, char* filterName)
{
	printf("%s filter %ld\n", filterName, result);
	BENCHMARK_PRINT_TIME(time);
}

static void benchmarkXTimes(int (*function) (int*), int times, int* data, char* functionName)
{
	int i;
	unsigned int lowestBenchmarkTime = 1 << ((sizeof(int) * 8) - 1);/*portable int max value*/
	for(i = 0; i < times; i++)
	{
		unsigned int benchmarkTime = (*function)(data);
		if(benchmarkTime < lowestBenchmarkTime)
		{
			lowestBenchmarkTime = benchmarkTime;
		}
	}
	printBenchmarkData(lowestBenchmarkTime, functionName);
}

static int benchmarkLowPassFilter(int* data)
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

static int benchmarkHighPassFilter(int* data)
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

static int benchmarkDerivativeSquareFilter(int* data)
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
		result += derivativeSquareMovingWindowFilter(y_1_1, y_1, y_3, y_4);
	}
	return BENCHMARK_TIME(startTime);
}

static int benchmarhWholeFilter(int* data)
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

static int benchmarkPeakSearcher(int* data)
{
	long startTime = BENCHMARK_START;
	result = 0;
	int peakTime = 0;
	for(int i = 0; i < ECG_10800K_LENGTH; i++)
	{
		if(foundPeak(data[i]))
		{
			Peak newPeak = getNewPeak();
			result += newPeak.RR;
			//simulate a found RR peak every 150 values
			if(peakTime >= 150)
			{
				setFoundNewRRPeak();
			}
		}
		peakTime++;
	}
	return BENCHMARK_TIME(startTime);
}

static int benchmarkRPeakFinder(int* data)
{
	return 0;
}

void runBenchmarks()
{
	int* data = loadDataArray("benchmark_files/ECG10800K.txt", ECG_10800K_LENGTH);
	if(data != NULL)
	{
		//benchmarkXTimes(&benchmarkLowPassFilter, 40, data, "low");
		//benchmarkXTimes(&benchmarkHighPassFilter, 40, data, "high");
		//benchmarkXTimes(&benchmarkDerivativeSquareFilter, 40, data, "derivative square moving window");
		//benchmarkXTimes(&benchmarhWholeFilter, 40, data, "whole");


		int* filteredData = malloc(ECG_10800K_LENGTH * sizeof(int));
		for(int i = 0; i < ECG_10800K_LENGTH; i++)
		{
			filteredData[i] = filterData(data[i]);
		}

		benchmarkXTimes(&benchmarkPeakSearcher, 40, filteredData, "peak searcher");
		free(filteredData);
		//benchmarkXTimes(&benchmarkPeakFinder, 20, data, "r peak finder");
	}

	free(data);
}





