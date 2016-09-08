#include "includes/inputManager.h"


FILE* startInputData(char* fileName)
{
	return fopen(fileName, "r");
}

short hasMoreData(FILE* file)
{
	return !feof (file);
}

short getNextData(FILE* file)
{
	short data;
	fscanf(file, "%d", &data);
	return data;
}

void stopInputData(FILE* file)
{
	fclose(file);
}
