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
void updateNewPeak(Peak newPeak, char isPulseUnstable, , unsigned short pulse);

#endif /* INCLUDES_UI_H_ */
