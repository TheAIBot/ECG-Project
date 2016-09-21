/*
 * peakSearcher.h
 *
 *  Created on: 2016/09/09
 *      Author: jesper
 */

#ifndef INCLUDES_PEAKSEARCHER_H_
#define INCLUDES_PEAKSEARCHER_H_

#include "peak.h"


Peak* getIfPeak(unsigned short newDataPoint);
void addRRTimeFromFormer();

#endif /* INCLUDES_PEAKSEARCHER_H_ */
