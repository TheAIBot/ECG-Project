#include <stdio.h>
#include "includes/ui.h"
#include "includes/peak.h"

#define MINUTE 1000 * 60 //number of milliseconds in  minute
#define MINIMUM_INTENSITY 2000

static int timeSinceStart = 0;

void tickUITimer()
{
	timeSinceStart++;
}

void updateNewPeak(Peak newPeak, char isPulseUnstable)
{
	//printf("Pulse: %d BPM\n", MINUTE / averageRRTIme);
	printf("\n\nIntensity: %hu\n", newPeak.intensity);
	printf("RR time:   %hu\n", newPeak.RR);
	printf("Time:      %d\n", timeSinceStart);
	if(newPeak.intensity < MINIMUM_INTENSITY)
	{
		printf("Warning! Intensity is low\n");
	}
	if(isPulseUnstable)
	{
		printf("Warning! Pulse is unstable\n");
	}
}
