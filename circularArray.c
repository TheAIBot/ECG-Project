#include <stdlib.h>
#include <stdio.h>
#include "includes/circularArray.h"

char initCircArray(CircularArray* circArray, int size, int startIndex)
{
	circArray->size = size;
	circArray->startIndex = startIndex;
	circArray->data = calloc(size, sizeof(int));

	if(circArray->data == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for circular array\n");
		return 0;
	}
	return 1;
}

int getCircArrayValue(CircularArray* circArray, int offset)
{
	return getArrayDataValue(circArray->data, circArray->startIndex, circArray->size, offset);
}


static int getOffsettedIndex(int startIndex, int arraySize, int offset)
{
	int correctIndex = startIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += arraySize;
	}
	else if(correctIndex >= arraySize)
	{
		correctIndex -= arraySize;
	}
	return correctIndex;
}

int getArrayDataValue(int data[], int startIndex, int arraySize, int offset)
{

	return data[getOffsettedIndex(startIndex, arraySize, offset)];
}

void insertCircArrayData(CircularArray* circArray, int newData)
{
	moveCircArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

void insertCircArrayDataAt(CircularArray* circArray, int newData, int offset)
{
	moveCircArrayStartIndex(circArray);
	circArray->data[getOffsettedIndex(circArray->startIndex, circArray->size, offset)] = newData;
}

void moveCircArrayStartIndex(CircularArray* circArray)
{
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size)
	{
		circArray->startIndex = 0;
	}
}

void freeCircArray(CircularArray* circArray)
{
	free(circArray->data);
}








