/*Made by the glorious Jesper*/

#ifndef INCLUDES_PEAKSEARCHER_H_
#define INCLUDES_PEAKSEARCHER_H_

#include "peak.h"

char foundPeak(unsigned short newDataPoint);
Peak getNewPeak();
void setFoundNewRRPeak(unsigned short newValue);
void setTimeSinceLastRPeakFound(unsigned short newValue);
int getTimeSinceLastRPeakFound();

#endif /* INCLUDES_PEAKSEARCHER_H_ */
