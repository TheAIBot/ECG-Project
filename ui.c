#include <stdio.h>
#include "includes/ui.h"
#include "includes/peak.h"

#define MINIMUM_INTENSITY 2000

static int timeSinceStart = 0;
//The assignment is so that it won't print the wrong warning, if the first peak found isn't a R-peak.
static Peak newestPeak = {.RR = 150, .intensity = MINIMUM_INTENSITY};

void tickUITimer() {
	timeSinceStart++;
}

void printAnyWarnings(char isPulseUnstable) {
	if (newestPeak.intensity < MINIMUM_INTENSITY) {
		printf("Warning! Intensity is low\n");
	}
	if (isPulseUnstable) {
		printf("Warning! Pulse is unstable\n");
	}
}

void printNewestPeakDetails(Peak newPeak, unsigned short pulse) {
	newestPeak = newPeak;

	printf("\n\nBpm:       %d\n", pulse);
	printf("Intensity: %hu\n", newPeak.intensity);
	printf("RR time:   %hu\n", newPeak.RR);
	printf("Time:      %d\n", timeSinceStart);

}
