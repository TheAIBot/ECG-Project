#include "includes/rawData.h"

#define RAW_DATA_SIZE 12

static short rawData[RAW_DATA_SIZE];
static short arrayStartIndex = 0;
short raw_y_12 = 0;

short getRawDataValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE;
	}
	return rawData[correctIndex];
}

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE)
	{
		arrayStartIndex = 0;
	}
}

void insertRawData(short data)
{
	moveArrayStartIndex();
	raw_y_12 = rawData[arrayStartIndex];
	rawData[arrayStartIndex] = data;
}

void resetRawBuffer()
{
	memset(rawData, 0, RAW_DATA_SIZE * sizeof(short));
	raw_y_12 = 0;
}
