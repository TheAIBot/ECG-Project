#include <string.h>
#include "includes/lowPassFilter.h"

#define RAW_DATA_SIZE_LOW 32
//represents the bits that are allowed to be set when the max index is RAW_DATA_SIZE_LOW - 1
//this only works when RAW_DATA_SIZE_LOW is a power of two
//this also assumes that the array start index is only incremented by one
#define ALLOWED_ENABLED_BITS_FOR_INDEX (RAW_DATA_SIZE_LOW - 1) // = 0x1F

static int rawLow[RAW_DATA_SIZE_LOW];
static unsigned short arrayStartIndex = 0;
//previous calculated value from lowPassFilter
static int y_1 = 0;
//formercy_1 value
static int y_2 = 0;
int low_y_32 = 0;

//offset is either negative or 0.
//return a previous low pass value.
int getLowPassValue(const int offset)
{
	//when arrayStartindex == RAW_DATA_SIZE_LOW the circular array start index has to begin
	//at 0 again and because the length of the array is a power of two it's possible to do just that
	//with a bitwise and which sets all the bits above the fifth bit to zero zero.
	return rawLow[(arrayStartIndex + offset) & ALLOWED_ENABLED_BITS_FOR_INDEX];
}

//moves the start index of the circular array rawLow forward.
//with looping when the end of the array has been passed.
static void moveArrayStartIndex()
{
	arrayStartIndex++;
	//when arrayStartindex == RAW_DATA_SIZE_LOW the circular array start index has to begin
	//at 0 again and because the length of the array is a power of two it's possible to do just that
	//with a bitwise and which sets all the bits above the fifth bit to zero zero.
	arrayStartIndex &= ALLOWED_ENABLED_BITS_FOR_INDEX;
}

//filters the arguments with a low pass filter.
//x is the newest raw value.
//x_6 is the raw value from 6 times ago.
//x_12 is the raw value from 12 times ago.
//returns a filtered result from the arguments.
int lowPassFilter(const int x, const int x_6, const int x_12)
{
	moveArrayStartIndex();

	//the low pass filter equation.
	int newY = (2 * y_1) - y_2 + ((x - (2 * x_6) + x_12) / 32);
	//int newY =   (2 * y_1) - y_2 + x - 2 * x_6 + x_12;
	//save the overridden value because so the high pass filter
	//doesn't have to use getLowPassValue to get it.
	low_y_32 = rawLow[arrayStartIndex];
	rawLow[arrayStartIndex] = newY;

	//save y_1 and y_2
	//because they are used the next time this method runs
	//by doing this two calls to getLowPassValue to get the
	//values are avoided.
	y_2 = y_1;
	y_1 = newY;
	return newY;
}

//sets all values in the circular array to 0
//and sets all cached values to 0.
void resetLowFilter()
{
	memset(rawLow, 0, RAW_DATA_SIZE_LOW * sizeof(int));
	y_1 = 0;
	y_2 = 0;
	low_y_32 = 0;
}





