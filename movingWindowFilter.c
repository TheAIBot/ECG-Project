#include "includes/movingWindowFilter.h"

#define RAW_DATA_SIZE_MOVING_WINDOW 13

static int totalValue = 0;

short movingWindowFilter(short sqrDiffrence)
{
	totalValue += sqrDiffrence;
	short newY = totalValue / N;
	return newY;
}

void resetMovingWindowBuffer()
{
	totalValue = 0;
}





