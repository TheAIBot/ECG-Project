#include "includes/highPassFilter.h"

#define Y_1_INDEX 0

short rawHigh[RAW_DATA_SIZE_HIGH];

static int x_17 = 0;

short highPassFilter(int x, int x_16, int x_32)
{

	short newY = rawHigh[Y_1_INDEX] - (x / 32) + x_16 - x_17 + (x_32 / 32);

	x_17 = x_16;

	rawHigh[4] = rawHigh[3];
	rawHigh[3] = rawHigh[2];
	rawHigh[2] = rawHigh[1];
	rawHigh[1] = rawHigh[0];
	rawHigh[0] = newY;

	return newY;
}

void resetHighBuffer()
{
	memset(rawHigh, 0, RAW_DATA_SIZE_HIGH * sizeof(short));
	x_17 = 0;
}






