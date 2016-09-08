/*
 * fileManager.h
 *
 *  Created on: 7 Sep 2016
 *      Author: feynman
 */
#include <stdio.h>

#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

FILE* startInputData(char* fileName);
short hasMoreData(FILE* file);
short getNextData(FILE* file);
void stopInputData(FILE* file);


#endif /* INPUTMANAGER_H_ */
