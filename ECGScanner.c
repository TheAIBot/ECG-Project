#include "includes/sensor.h"
#include "includes/filter.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/rPeakFinder.h"
#include "includes/ui.h"

void runScannerOnce(int sensorValue)
{
	unsigned short filteredValue = filterData((sensorValue));

	if(foundPeak(filteredValue))
	{
		Peak newPeak = getNewPeak();
		if(isRPeak(&newPeak)){
			//TODO verify this is correct
			setFoundNewRRPeak();
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
		int newData = getNextData(file);
		if(!hasMoreData(file))
		{
			break;
		}

		runScannerOnce(newData);
	}
	stopInputData(file);
}
