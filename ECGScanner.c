#include "includes/sensor.h"
#include "includes/filter.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/peakCircularArray.h"
#include "includes/rPeakFinder.h"
#include "includes/ui.h"

#define TIME_BETWEEN_SENSOR_READ_IN_MS 4

void runScannerOnce(int sensorValue)
{
	unsigned short filteredValue = filterData((sensorValue));

	if(foundPeak(filteredValue))
	{
		Peak newPeak = getNewPeak();
		if(isRPeak(newPeak)){
			//TODO verify this is correct
			setFoundNewRRPeak();
			Peak correctPeak = getPeakCircArrayValue(getTrueRPeaksArray(), 0);
			updateNewPeak(correctPeak, isPulseUnstable());
		}
	}
	tickUITimer();
}

void runScanner(){
	FILE* file = startInputData("ECG.txt");
	//FILE* file = startInputData("benchmark_files/ECG10800K.txt");

	initializeRPeakFinder();

	while(1)	{
		int newData = getNextData(file);
		if(!hasMoreData(file))		{
			break;
		}

		runScannerOnce(newData);
	}
	stopInputData(file);
}
