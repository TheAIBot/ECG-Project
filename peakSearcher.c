#include "string.h"
#include "includes/peakSearcher.h"
#include "includes/circularArray.h"
#include "includes/peak.h"

#define PEAK_AREA 5 //The size of the arrea investigated during the checks for if a value is a peak.
#define MIDDLE_INDEX (PEAK_AREA / 2) //The middle index of last5Values.
//Minimum time required to pass since the last detection of a true R-peak, before a new peak can be registrated.
#define MINIMUM_TIME_BETWEEM_R_PEAKS 60 - PEAK_AREA
/*Indicates the RR time since the last found true R-peak.
 *It starts at -(PEAK_AREA - 1)/2 + , because of the delay of (PEAK_AREA - 1)/2
  */
static unsigned short timeSinceLastRPeak = -MIDDLE_INDEX;
//Array of the last five intensity found. Used to check if the middle value is a peak.
static unsigned short last5Values[PEAK_AREA] = {0};

/* Returns if the middle value in last5Values is a peak.
 * This corresponds to it being bigger than the last value/intensity bigger than itself,
 * greater than 100, and being greater than or equal to the two values before it,
 * and greater than (but not equal to) the two values after itself.
 *
 * returns 1 if the middle value is a peak, else 0*/
static char isPeak(){
	//Used to check that if a datapoint corresponds to a peak, but it is on a plateu,
	//that the value just before the plateu is less than those on the lateu, so only a true peak is found.
	static unsigned short formerDifferentValue = 0;

	//TODO make into a macro?
	unsigned short potentialPeak = last5Values[MIDDLE_INDEX];

	//Sets formerDifferentValue so that on a plateau (even of lenght 1, meaning one datapoint)
	//it is the value before the plateau
	if (last5Values[MIDDLE_INDEX - 1] != potentialPeak)
		formerDifferentValue = last5Values[MIDDLE_INDEX - 1];

	//If the intensity of the datapoint is less than 100, it cannot be a true R-peak.
	if (last5Values[MIDDLE_INDEX] < 100)
		return 0;
	//Makes the last checks.
	return (last5Values[MIDDLE_INDEX - 2] <= potentialPeak &&
			last5Values[MIDDLE_INDEX - 1] <= potentialPeak &&
			last5Values[MIDDLE_INDEX + 1] <  potentialPeak &&
			last5Values[MIDDLE_INDEX + 2] <  potentialPeak &&
			formerDifferentValue          <  potentialPeak);
}

/*Returns whether or not the second former datapoint before the given one, is a peak.
 *
 * unsigned short newDataPoint; The given datapoint, that is the intensity of the filtered data measured from the electrodes.
 *
 * Returns 1 if the second former datapoint before the given one is a peak, else 0*/
char foundPeak(unsigned short newDataPoint){
	timeSinceLastRPeak++;

	if(timeSinceLastRPeak <= MINIMUM_TIME_BETWEEM_R_PEAKS) //TODO some discussion with this - Jesper
		return 0;
	//Move the whole array back once so the new data can be inserted at the last index
	memcpy( last5Values, last5Values + 1, sizeof(last5Values));
	last5Values[PEAK_AREA - 1] = newDataPoint;

	return isPeak();
}

/*Returns the middle value of last5Values as a Peak.*/
Peak getNewPeak(){
	return (Peak){last5Values[MIDDLE_INDEX], timeSinceLastRPeak};
}

/*Sets timeSinceLastRPeak to a given value, in case a new true R-peak has been found.
 *
 * unsigned short newValue; the given value.
 *TODO update the repport, not what is written there*/
void setTimeSinceLastRPeakFound(unsigned short newValue){
	timeSinceLastRPeak = newValue;
}

int getTimeSinceLastRPeakFound(){
	return timeSinceLastRPeak;
}
