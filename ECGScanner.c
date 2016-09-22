#include "includes/sensor.h"
#include "includes/filter.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/peakCircularArray.h"
#include "includes/rPeakFinder.h"
#include "includes/ui.h"

#define TIME_BETWEEN_SENSOR_READ_IN_MS 4
int numberOfRPeaksFound = 0;

void runScannerOnce(int sensorValue)
{
	unsigned short filteredValue = filterData((sensorValue));

	Peak* newPeak = getIfPeak(filteredValue);
	if(newPeak != NULL)
	{
		if(isRPeak(*newPeak)){
			//TODO verify this is correct. It is not - Jesper
			char numberNewRPeaksFound = getNumberNewRPeaksFound();
			for(int i = numberNewRPeaksFound - 1; i >= 0; i--){
				//Note that isPulseUnstable() will only return if the latest pulse was unstable.
				updateNewPeak(getPeakCircArrayValue(getTrueRPeaksArray(), -i), isPulseUnstable());
				numberOfRPeaksFound++;
			}
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
	printf("The number of R peaks found is: %d",numberOfRPeaksFound);
	stopInputData(file);
}
