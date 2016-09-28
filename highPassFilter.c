#include <string.h>
#include "includes/highPassFilter.h"


#define RAW_DATA_SIZE_HIGH 5
#define Y_1_INDEX 0

short rawHigh[RAW_DATA_SIZE_HIGH];
static int x_17 = 0;


//filters the arguments with a high pass filter.
//x is the newest low pass filter value.
//x_16 is the low pass filter value from 16 times ago.
//x_32 is the low pass filter value from 32 times ago.
//returns a filtered result from the arguments.
short highPassFilter(const int x, const int x_16, const int x_32)
{
	//high pass filter equation
	short newY = GET_HIGH_PASS_VALUE(Y_1_INDEX) - (x / 32) + x_16 - x_17 + (x_32 / 32);

	//save x_16 as it will be x_17 the next time this filter is run
	x_17 = x_16;
	//move all filtered values one up in the array rawHigh
	memcpy( rawHigh + 1, rawHigh, sizeof(rawHigh));

	rawHigh[0] = newY;
	return newY;
}

//sets all values in the array to 0
//and sets all cached values to 0.
void resetHighFilter()
{
	memset(rawHigh, 0, RAW_DATA_SIZE_HIGH * sizeof(short));
	x_17 = 0;
}






