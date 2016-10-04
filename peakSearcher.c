#include <string.h>
#include <stdlib.h>
#include "includes/peakSearcher.h"
#include "includes/circularArray.h"
#include "includes/peak.h"

//the number - itself of numbers the peak should be larger of
//to be a peak. Has to be an odd number
#define PEAK_AREA 5
//the middle index of the array last5Values
#define MIDDLE_INDEX (PEAK_AREA / 2)
//with a time between potential peaks at 4ms and an assumption that 250 BPM is
//the max which is humanly possible the minimum amount of time between RR peaks is 60 * 4ms = 240ms.
//60 is subtracted the sizeof the array so he array is filled up with fresh values when it reaches the
//time where a new potential RR peak is physically possible
#define MINIMUM_TIME_BETWEEM_RR_PEAKS 60 - PEAK_AREA
#define MINIMUM_ALLOWED_PEAK_INTENSITY 100

//There is a delay of PEAK_AREA / 2 input values on the determination of a peak so to start out with it has to start
//with that as negative for the first RR time to be correct.
static unsigned short timeSinceLastRRPeak = -MIDDLE_INDEX;

//array of the last 5 filtered values
static unsigned short last5Values[PEAK_AREA] = { 0 };

static char isPeak() {
	//contains the last value that wasn't the same as th new value
	static unsigned short formerDifferentValue = 0;

	//the middle value is the value that has potential to be the peak
	//because the values around is the ncomparedagainst the middle value
	unsigned short potentialPeak = last5Values[MIDDLE_INDEX];

	//At the end of a plateau this makes sure that the start of the plateau is lower
	//than potential peak because formerDifferentValue always
	//holds the last value that was different than potentialPeak
	formerDifferentValue = (last5Values[MIDDLE_INDEX - 1] != potentialPeak)? last5Values[MIDDLE_INDEX - 1] : formerDifferentValue;

	//is potential peak larger than all values or
	//is potential peak at the end of a plateau
	return (last5Values[MIDDLE_INDEX + 2] <  potentialPeak &&
			last5Values[MIDDLE_INDEX + 1] <  potentialPeak &&
			last5Values[MIDDLE_INDEX - 1] <= potentialPeak &&
			last5Values[MIDDLE_INDEX - 2] <= potentialPeak &&
			formerDifferentValue 		  <  potentialPeak);
}

//newDataPoint will be a potential peak in MIDDLE_INDEX more times this function has run
//returns whether a peak was found or not
char foundPeak(unsigned short newDataPoint) {
	timeSinceLastRRPeak++;

	//no need to check if there is a peak if it isn't physically possible for one to be there
	if (timeSinceLastRRPeak <= MINIMUM_TIME_BETWEEM_RR_PEAKS) { //(*)Change back to MINIMUM_TIME_BETWEEM_RR_PEAKS
		return 0;
	}

	//if the intensity is too low then it's not possible for the peak to be an r peak
	//it's not nessesary to update ormerDifferentValue before this because when potential peak passes this
	//it will also,the first time, update formerDifferentValue.
	if (newDataPoint < MINIMUM_ALLOWED_PEAK_INTENSITY) {
		return 0;
	}

	//Move the whole array back once so the new data can be inserted at the last index
	memcpy(last5Values, last5Values + 1, sizeof(last5Values));
	last5Values[PEAK_AREA - 1] = newDataPoint;

	return isPeak();
}

/*Returns the middle value of last5Values as a Peak.*/
Peak getNewPeak() {
	return (Peak ) { last5Values[MIDDLE_INDEX], timeSinceLastRRPeak } ;
}

/*Sets timeSinceLastRPeak to a given value, in case a new true R-peak has been found.
*
* unsigned short newValue; the given value.
* */
void setTimeSinceLastRPeakFound(unsigned short newValue) {
	timeSinceLastRRPeak = newValue;
}

int getTimeSinceLastRPeakFound() {
	return timeSinceLastRRPeak;
}
