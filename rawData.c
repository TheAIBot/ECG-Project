#include <string.h>
#include "includes/rawData.h"

#define RAW_DATA_SIZE 16
#define ALLOWED_ENABLED_BITS_FOR_INDEX (RAW_DATA_SIZE - 1) // 0x0F

static short rawData[RAW_DATA_SIZE];
static unsigned short arrayStartIndex = 0;

//offset is either negative or 0.
//return a previous low pass value.
short getRawDataValue(const int offset)
{
	//when arrayStartindex == RAW_DATA_SIZE the circular array start index has to begin
	//at 0 again and because the length of the array is a power of two it's possible to do just that
	//with a bitwise and which sets all the bits above the fifth bit to zero zero.
	return rawData[(arrayStartIndex + offset) & ALLOWED_ENABLED_BITS_FOR_INDEX];
}

//moves the start index of the circular array rawLow forward.
//with looping when the end of the array has been passed.
static void moveArrayStartIndex()
{
	arrayStartIndex++;
	//when arrayStartindex == RAW_DATA_SIZE the circular array start index has to begin
	//at 0 again and because the length of the array is a power of two it's possible to do just that
	//with a bitwise and which sets all the bits above the fifth bit to zero zero.
	arrayStartIndex &= ALLOWED_ENABLED_BITS_FOR_INDEX;
}

//inserts a short value into the circular array.
//data is the value to insert.
void insertRawData(const short data)
{
	moveArrayStartIndex();
	//save the overridden value because so the low pass filter
	//doesn't have to use getLowPassValue to get it.
	rawData[arrayStartIndex] = data;
}

//sets all values in the circular array to 0
//and sets all cached values to 0.
void resetRawBuffer()
{
	memset(rawData, 0, RAW_DATA_SIZE * sizeof(short));
}
