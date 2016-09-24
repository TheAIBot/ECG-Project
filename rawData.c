#include <string.h>
#include "includes/rawData.h"

#define RAW_DATA_SIZE 12

static short rawData[RAW_DATA_SIZE];
static short arrayStartIndex = 0;
//instead of a filter calling getRawDataValue to get the last value
//from rawData it can directly fetch it via this variable.
short raw_y_12 = 0;

//offset is either negative or 0.
//return a previous raw data value.
short getRawDataValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE;
	}
	return rawData[correctIndex];
}

//moves the start index of the circular array rawData forward
//with looping when the end of the array has been passed.
static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE)
	{
		arrayStartIndex = 0;
	}
}

//inserts a short value into the circular array.
//data is the value to insert.
void insertRawData(const short data)
{
	moveArrayStartIndex();
	//save the overridden value because so the low pass filter
	//doesn't have to use getLowPassValue to get it.
	raw_y_12 = rawData[arrayStartIndex];
	rawData[arrayStartIndex] = data;
}

//sets all values in the circular array to 0
//and sets all cached values to 0.
void resetRawBuffer()
{
	memset(rawData, 0, RAW_DATA_SIZE * sizeof(short));
	raw_y_12 = 0;
}
