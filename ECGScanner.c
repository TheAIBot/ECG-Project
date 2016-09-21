#include "includes/sensor.h"
#include "includes/filter.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/rPeakFinder.h"
#include "includes/ui.h"

#define TIME_BETWEEN_SENSOR_READ_IN_MS 4

void runScannerOnce(int sensorValue)
{
	unsigned short filteredValue = filterData((sensorValue));

	Peak* newPeak = getIfPeak(filteredValue);
	if(newPeak != NULL)
	{
		if(isRPeak(newPeak)){
			//TODO verify this is correct
			updateNewPeak(newPeak, isPulseUnstable());
		}
	}
	ShowNormalInformation();
}

void runScanner()
{
	FILE* file = startInputData("ECG.txt");

	initializeRPeakFinder();

	while(1)
	{
		int newData = waitForNextValue(file, TIME_BETWEEN_SENSOR_READ_IN_MS);
		if(!hasMoreData(file))
		{
			break;
		}

		runScannerOnce(newData);
	}
	stopInputData(file);
}
