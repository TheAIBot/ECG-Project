#include "string.h"
#include "includes/peakSearcher.h"
#include "includes/circularArray.h"
#include "includes/peak.h"

#define PEAK_AREA 5
#define MIDDLE_INDEX (PEAK_AREA / 2)
#define MINIMUM_TIME_BETWEEM_RR_PEAKS 60 - PEAK_AREA

static unsigned short timeSinceLastRRPeak = -MIDDLE_INDEX; /*It starts at -(PEAK_AREA - 1)/2 + , because of the delay of (PEAK_AREA - 1)/2*/

static unsigned short last5Values[PEAK_AREA] = {0};

static char isPeak()
{
	static unsigned short formerDifferentValue = 0;
	/*TODO Talk about the following extra check with the teachers*/
	/*To avoid registrating a peak in case of a fall, then a plateu, then a fall in values.*/
	unsigned short potentialPeak = last5Values[MIDDLE_INDEX];

	if (last5Values[MIDDLE_INDEX - 1] != potentialPeak)
	{
		formerDifferentValue = last5Values[MIDDLE_INDEX - 1];
	}

	//TODO make these values a macro
	if (last5Values[MIDDLE_INDEX] < 100)
	{
		return 0;
	}

	return (last5Values[MIDDLE_INDEX - 2] <= potentialPeak &&
			last5Values[MIDDLE_INDEX - 1] <= potentialPeak &&
			last5Values[MIDDLE_INDEX + 1] <  potentialPeak &&
			last5Values[MIDDLE_INDEX + 2] <  potentialPeak &&
			formerDifferentValue          <  potentialPeak);
}

//TODO add description
char foundPeak(unsigned short newDataPoint)
{
	timeSinceLastRRPeak++;

	if(timeSinceLastRRPeak <= MINIMUM_TIME_BETWEEM_RR_PEAKS)
	{
		return 0;
	}
	//Move the whole array back once so the new data can be inserted at the last index
	memcpy( last5Values, last5Values + 1, sizeof(last5Values));
	last5Values[PEAK_AREA - 1] = newDataPoint;

	return isPeak();
}

Peak getNewPeak()
{
	return (Peak){last5Values[MIDDLE_INDEX], timeSinceLastRRPeak};
}

void setFoundNewRRPeak()
{
	timeSinceLastRRPeak = 0;
}
