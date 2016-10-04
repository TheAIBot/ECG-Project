#include "includes/sensor.h"
#include "includes/filter.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/peakCircularArray.h"
#include "includes/peakAvgCircularArray.h"
#include "includes/rPeakFinder.h"
#include "includes/ui.h"

#define TIME_BETWEEN_SENSOR_READ_IN_MS 4

void runScannerOnce(int sensorValue, char showUI)
{
	unsigned short filteredValue = filterData((sensorValue));

	if(foundPeak(filteredValue))	{
		Peak newPeak = getNewPeak();
		if(isRPeak(newPeak)){
			Peak correctPeak = getPeakAvgCircValue(getTrueRPeaksArray(), 0);
			printNewestPeakDetails(correctPeak, getPulse(), showUI);
		}
		printAnyWarnings(isPulseUnstable(), showUI);
	}
	tickUITimer();
}

void runScanner(char* filepath, char showUI){
	FILE* file = startInputData(filepath);
	if(file == NULL)	{
		return;
	}

	initializeRPeakFinder();

	while(1)	{
		int newData = getNextData(file);
		if(!hasMoreData(file))		{
			break;
		}

		runScannerOnce(newData, showUI);
	}
	stopInputData(file);
	freeRPeakFinder();
}
