#include "string.h"
#include "includes/peakSearcher.h"
#include "includes/circularArray.h"
#include "includes/peak.h"

#define PEAK_AREA 5
#define MIDDLE_INDEX (PEAK_AREA / 2)

/*TODO Think about removing the value below, and calcultating it on the go. Discuss with Andreas - don't remove - Jesper*/
static unsigned short lastPeakTime = 0;
static unsigned short timeSinceLastPeak = -(PEAK_AREA - 1) / 2 + 1; /*It starts at -(PEAK_AREA - 1)/2 + , because of the delay of (PEAK_AREA - 1)/2*/

static unsigned short last5Values[PEAK_AREA] = {0};

static char isPeak()
{
	static unsigned short formerDifferentValue = 0;
	/*TODO Talk about the following extra check with the teachers*/
	/*To avoid registrating a peak in case of a fall, then a plateu, then a fall in values.*/
	if (last5Values[MIDDLE_INDEX - 1] != last5Values[MIDDLE_INDEX])
	{
		formerDifferentValue = last5Values[MIDDLE_INDEX - 1];
	}

	//TODO make these values a macro
	if (last5Values[MIDDLE_INDEX] < 100 || timeSinceLastPeak <= 20)
	{
		return 0;
	}

	if (last5Values[MIDDLE_INDEX - 2] <= last5Values[MIDDLE_INDEX] &&
		last5Values[MIDDLE_INDEX - 1] <= last5Values[MIDDLE_INDEX] &&
		last5Values[MIDDLE_INDEX + 1] < last5Values[MIDDLE_INDEX] &&
		last5Values[MIDDLE_INDEX + 2] < last5Values[MIDDLE_INDEX] &&
		formerDifferentValue < last5Values[MIDDLE_INDEX])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//TODO add description
Peak* getIfPeak(unsigned short newDataPoint){
	/*TODO Fix recording first data point as a peak.
	 *  Needs to be discussed with the teacher or the teaching assistants. - Jesper
	 */

	//Move the whole array back once so the new data can be inserted at the last index
	memcpy( last5Values, last5Values + 1, sizeof(last5Values));
	last5Values[PEAK_AREA - 1] = newDataPoint;

	if (isPeak())
	{
		Peak newPeak = {last5Values[MIDDLE_INDEX], timeSinceLastPeak};
		Peak* returnablePeak = malloc(sizeof(Peak));
		memcpy(returnablePeak, &newPeak, sizeof(Peak));


		lastPeakTime = timeSinceLastPeak;
		timeSinceLastPeak = 0;

		return returnablePeak;
	}

	timeSinceLastPeak++;
	return NULL;
}

void addRRTimeFromFormer()
{
	timeSinceLastPeak += lastPeakTime;
}
