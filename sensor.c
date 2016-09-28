#include <stdlib.h>
#include "includes/sensor.h"
#include "includes/benchmarkTimer.h"


FILE* startInputData(char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open file %s\n", fileName);
	}
	return file;
}

char hasMoreData(FILE* file)
{
	return !feof(file);
}

int getNextData(FILE* file)
{
	signed int data;
	fscanf(file, "%d", &data);
	return data;
}

void getPeakData(FILE* file, int* time, int* measurement)
{
	fscanf(file, "%d %d", time, measurement);
}

int* loadPeakData(FILE* file, int fileSizeInLines)
{
	//first array is time and second is measurements
	int* timeAndMeasurements = malloc(2 * fileSizeInLines * sizeof(int));
	for(int i = 0; i < fileSizeInLines; i++)
	{
		getPeakData(file, &timeAndMeasurements[i], &timeAndMeasurements[i + fileSizeInLines]);
	}
	return timeAndMeasurements;

}

void stopInputData(FILE* file)
{
	fclose(file);
}

int* loadDataArray(char* filename, int arrayLength)
{
	FILE* inputFile = startInputData(filename);

	if(inputFile == NULL)
	{
		fprintf(stderr, "Failed to load %s", filename);
		return NULL;
	}

	int* inputArray = malloc(arrayLength * sizeof(int));

	if(inputArray == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for %s array", filename);
		return NULL;
	}
	int i = 0;
	for(; i < arrayLength; i++)
	{
		inputArray[i] = getNextData(inputFile);
	}

	stopInputData(inputFile);

	return inputArray;
}
