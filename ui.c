#include <stdio.h>
#include "includes/ui.h"
#include "includes/peak.h"

#define MINUTE 1000 * 60 //number of milliseconds in  minute
#define MINIMUM_INTENSITY 2000

static Peak newestPeak = {0};
static int timeSinceStart = 0;

void ShowNormalInformation()
{
	timeSinceStart++;
}

void updateNewPeak(Peak newPeak, char isPulseUnstable)
{
	memcpy(&newestPeak, &newPeak, sizeof(Peak));

	//printf("Pulse: %d BPM\n", MINUTE / averageRRTIme);
	printf("\n\nIntensity: %hu\n", newestPeak.intensity);
	printf("RR time:   %hu\n", newestPeak.RR);
	printf("Time:      %d\n", timeSinceStart);
	if(newestPeak.intensity < MINIMUM_INTENSITY)
	{
		printf("Warning! Intensity is low\n");
	}
	if(isPulseUnstable)
	{
		printf("Warning! Pulse is unstable\n");
	}
}
