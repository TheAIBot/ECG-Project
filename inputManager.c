#include "includes/inputManager.h"


FILE* startInputData(char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open file %s.\n", fileName);
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

void stopInputData(FILE* file)
{
	fclose(file);
}

int* loadDataArray(char* filename, int arrayLength)
{
	FILE* inputFile = startInputData(filename);

	if(inputFile == NULL)
	{
		printf("Failed to load %s", filename);
		return NULL;
	}

	int* inputArray = malloc(arrayLength * sizeof(int));

	if(inputArray == NULL)
	{
		printf("Failed to allocate memory for %s array", filename);
		return NULL;
	}

	for(int i = 0; i < arrayLength; i++)
	{
		inputArray[i] = getNextData(inputFile);
	}

	stopInputData(inputFile);

	return inputArray;
}
