#include "includes/rawData.h"

#define RAW_DATA_SIZE 13

short rawData[RAW_DATA_SIZE];
short arrayStartIndex1 = 0;

short getRawDataValue(int offset)
{
	short correctIndex = arrayStartIndex1 + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE;
	}
	return rawData[correctIndex];
}

void moveArrayStartIndex1()
{
	arrayStartIndex1++;
	if(arrayStartIndex1 == RAW_DATA_SIZE)
	{
		arrayStartIndex1 = 0;
	}
}

void insertRawData(short data)
{
	moveArrayStartIndex1();
	rawData[arrayStartIndex1] = data;
}
