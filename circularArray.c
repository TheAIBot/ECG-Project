#include <stdlib.h>
#include "includes/circularArray.h"

void initCircularArray(struct CircularArray* circArray, int size, int startIndex)
{
	circArray->size = size;
	circArray->startIndex = startIndex;
	circArray->data = calloc(size, sizeof(int));
}

int getDataValue(struct CircularArray* circArray, int offset)
{
	return getArrayDataValue(circArray->data, circArray->startIndex, circArray->size, offset);
}

int getArrayDataValue(int data[], int startIndex, int arraySize, int offset)
{
	int correctIndex = startIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += arraySize;
	}
	return data[correctIndex];
}

void insertData(struct CircularArray* circArray, int newData)
{
	moveArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

void moveArrayStartIndex(struct CircularArray* circArray)
{
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size)
	{
		circArray->startIndex = 0;
	}
}








