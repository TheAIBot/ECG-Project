#include "includes/inputManager.h"


FILE* startInputData(char* fileName)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open file %s.\n", fileName);
	}
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
