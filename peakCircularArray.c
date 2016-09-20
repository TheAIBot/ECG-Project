#include <stdlib.h>
#include <stdio.h>
#include "includes/peakCircularArray.h"

typedef struct TPeak Peak;
typedef struct TPeakCircularArray PeakCircularArray;


char initPeakCircArray(PeakCircularArray* circArray, int size, int startIndex)
{
	circArray->size = size;
	circArray->startIndex = startIndex;
	circArray->data = calloc(size, sizeof(Peak*));
	if(circArray->data == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for peak circular array");
		return 0;
	}
	return 1;
}

Peak* getPeakCircArrayValue(PeakCircularArray* circArray, int offset){
	int correctIndex = circArray->startIndex + offset;
	if(correctIndex < 0)
		correctIndex += circArray->size;
	else if (correctIndex >= circArray->size)
		correctIndex -= circArray->size;
	return circArray->data[correctIndex];
}
//TODO delete
Peak* getPeakArrayDataValue(Peak* data[], int startIndex, int arraySize, int offset)
{
	int correctIndex = startIndex + offset;
	if(correctIndex < 0)
		correctIndex += arraySize;
	else if (correctIndex >= arraySize)
		correctIndex -= arraySize;
	return data[correctIndex];
}

void insertPeakCircArrayData(PeakCircularArray* circArray, Peak* newData){
	moveCircArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

void movePeakCircArrayStartIndex(PeakCircularArray* circArray)
{
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size){
		circArray->startIndex = 0;
	}
}

void freePeakCircArray(PeakCircularArray* circArray)
{
	free(circArray->data);
}








