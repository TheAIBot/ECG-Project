#include "includes/highPassFilter.h"


#define RAW_DATA_SIZE_HIGH 5
#define Y_1_INDEX 0

short rawHigh[RAW_DATA_SIZE_HIGH];
static int x_17 = 0;

short highPassFilter(int x, int x_16, int x_32)
{
	short newY = GET_HIGH_PASS_VALUE(Y_1_INDEX) - (x / 32) + x_16 - x_17 + (x_32 / 32);
	x_17 = x_16;
	memcpy( rawHigh + 1, rawHigh, sizeof(rawHigh));
	rawHigh[0] = newY;
	return newY;
}

void resetHighFilter()
{
	memset(rawHigh, 0, RAW_DATA_SIZE_HIGH * sizeof(short));
	x_17 = 0;
}






