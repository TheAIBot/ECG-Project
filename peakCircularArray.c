#include <stdlib.h>
#include <stdio.h>
#include "includes/peakCircularArray.h"

/*Initializes a given PeakCircularArray (pointer), to have a given size and startindex.
 *
 * PeakCircularArray* const circArray; Pointer to the circular array that should be initialized.
 * const int size; The size that the initialized array should have.
 * const int startIndex; The startindex that the initialized circular array should have.
 * */
char initPeakCircArray(PeakCircularArray* const circArray, const int size, const int startIndex){
	circArray->size = size;
	circArray->startIndex = startIndex;
	circArray->data = malloc(size * sizeof(Peak)); //Allocates memory for the array.
	if(circArray->data == NULL){
		fprintf(stderr, "Failed to allocate memory for peak circular array");
		return 0;
	}
	return 1;
}

/* Gets the element in the circular array (pointer), that has a given offset from the startindex.
 * The offset must be negative, and greater than or equal to minus the size of the circular array.
 *
 * returns The peak with the given offset from the current/startindex.
 * */
Peak getPeakCircArrayValue(const PeakCircularArray* circArray, const int offset){
	int correctIndex = circArray->startIndex + offset;
	//assumes that offset is always negative or 0
	if(correctIndex < 0) {
		correctIndex += circArray->size;
	}
	return circArray->data[correctIndex];
}

/*Moves one forward in the circular array. If it is at the end, it loops back to the start*/
static void movePeakCircArrayStartIndex(PeakCircularArray* const circArray){
	circArray->startIndex++;
	if(circArray->startIndex == circArray->size) {
		circArray->startIndex = 0;
	}
}

/*Inserts a new element in a given circular array (pointer), at the position after the current startindex,
 *with a loopback if the startindex is at the last position in the array.
  *
 * PeakCircularArray* const circArray; pointer for the new circular array, wherein the data point should be inserted.
 * const Peak newData; the element to be inserted.
  */
void insertPeakCircArrayData(PeakCircularArray* const circArray, const Peak newData){
	movePeakCircArrayStartIndex(circArray);
	circArray->data[circArray->startIndex] = newData;
}

/*Frees the memory in the circular array*/
void freePeakCircArray(PeakCircularArray* const circArray){
	free(circArray->data);
}








