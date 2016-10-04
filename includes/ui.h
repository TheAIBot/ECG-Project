/*
 * ui.h
 *
 *  Created on: 19 Sep 2016
 *      Author: feynman
 */

#ifndef INCLUDES_UI_H_
#define INCLUDES_UI_H_

#include "peak.h"

void tickUITimer();
void printAnyWarnings(char isPulseUnstable, char showUI);
void printNewestPeakDetails(Peak newPeak, unsigned short pulse, char showUI);

#endif /* INCLUDES_UI_H_ */
