#include "includes/rawData.h"

static short rawData[RAW_DATA_SIZE];
static short arrayStartIndex = 0;

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
	rawData[arrayStartIndex] = data;
}

void resetRawBuffer()
{
	memset(rawData, 0, RAW_DATA_SIZE * sizeof(short));
}
