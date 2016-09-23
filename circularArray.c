#include <stdlib.h>
#include <stdio.h>
#include "includes/circularArray.h"

char initCircArray(CircularArray* const circArray, const int size, const int startIndex)
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

int getCircArrayValue(const CircularArray* circArray, const int offset)
{
	return getArrayDataValue(circArray->data, circArray->startIndex, circArray->size, offset);
}


static int getOffsettedIndex(const int startIndex, const int arraySize, const int offset)
{
	int correctIndex = startIndex + offset;
	//assumes that offset is always negative or 0
	if(correctIndex < 0)
	{
		correctIndex += arraySize;
	}
	return correctIndex;
}

int getArrayDataValue(const int data[], const int startIndex, const int arraySize, const int offset)
{

	return data[getOffsettedIndex(startIndex, arraySize, offset)];
}

static void moveCircArrayStartIndex(CircularArray* const circArray)
{
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size)
	{
		circArray->startIndex = 0;
	}
}

void insertCircArrayData(CircularArray* circArray, const int newData)
{
	moveCircArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

void insertCircArrayDataAt(CircularArray* circArray, const int newData, const int offset)
{
	moveCircArrayStartIndex(circArray);
	circArray->data[getOffsettedIndex(circArray->startIndex, circArray->size, offset)] = newData;
}

void freeCircArray(CircularArray* const circArray)
{
	free(circArray->data);
}








