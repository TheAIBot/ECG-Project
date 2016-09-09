#include "includes/benchmarkCircularArray.h"

int getBenchmarkDataValue(int data[], int index, int arraySize, int offset)
{
	int correctIndex = index + offset;
	if(correctIndex < 0)
	{
		correctIndex += arraySize;
	}
	return data[correctIndex];
}

void insertBenchmarkData(int data[], int newData, int index)
{
	data[index] = newData;
}
