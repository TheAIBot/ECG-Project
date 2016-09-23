#include <stdlib.h>
#include <stdio.h>
#include "includes/peakCircularArray.h"


char initPeakCircArray(PeakCircularArray* const circArray, const int size, const int startIndex){
	circArray->size = size;
	circArray->startIndex = startIndex;
	circArray->data = malloc(size * sizeof(Peak));
	if(circArray->data == NULL){
		fprintf(stderr, "Failed to allocate memory for peak circular array");
		return 0;
	}
	return 1;
}

Peak getPeakCircArrayValue(const PeakCircularArray* circArray, const int offset){
	int correctIndex = circArray->startIndex + offset;
	//assumes that offset is always negative or 0
	if(correctIndex < 0) {
		correctIndex += circArray->size;
	}
	return circArray->data[correctIndex];
}

static void movePeakCircArrayStartIndex(PeakCircularArray* const circArray){
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size) {
		circArray->startIndex = 0;
	}
}

void insertPeakCircArrayData(PeakCircularArray* const circArray, const Peak newData){
	movePeakCircArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

void freePeakCircArray(PeakCircularArray* const circArray){
	free(circArray->data);
}








