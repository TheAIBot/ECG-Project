/*Made by the glorious Andreas*/

#include <stdio.h>

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

FILE* startInputData(char* fileName);
char hasMoreData(FILE* file);
int getNextData(FILE* file);
int* loadPeakData(FILE* file, int fileSizeInLines);
void stopInputData(FILE* file);
int* loadDataArray(char* filename, int arrayLength);


#endif /* INPUTMANAGER_H_ */
