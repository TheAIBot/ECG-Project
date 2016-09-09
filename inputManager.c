#include "includes/inputManager.h"


FILE* startInputData(char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open file %s.\n", fileName);
	}
}

short hasMoreData(FILE* file)
{
	return !feof(file);
}

short getNextData(FILE* file)
{
	short data;
	fscanf(file, "%hd", &data);
	return data;
}

void getPeakData(FILE* file, int* time, int* measurement)
{
	fscanf(file, "%d %d", *time, *measurement);
}

void stopInputData(FILE* file)
{
	fclose(file);
}
