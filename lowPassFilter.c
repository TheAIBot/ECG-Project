#include "includes/lowPassFilter.h"

#define RAW_DATA_SIZE_LOW 33

short rawLow[RAW_DATA_SIZE_LOW];
short arrayStartIndex2 = 0;

short getLowPassValue(int offset)
{
	short correctIndex = arrayStartIndex2 + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_LOW;
	}
	return rawLow[correctIndex];
}

void moveArrayStartIndex2()
{
	arrayStartIndex2++;
	if(arrayStartIndex2 == RAW_DATA_SIZE_LOW)
	{
		arrayStartIndex2 = 0;
	}
}

short lowPassFilter(short x, short x_6, short x_12)
{
	moveArrayStartIndex2();

	short y_1 = getLowPassValue(-1);
	short y_2 = getLowPassValue(-2);
	/*
	 * printf("%hd\n", x);
	 * printf("%hd\n", x_6);
	 * printf("%hd\n", x_12);
	 * printf("%hd\n", y_1);
	 * printf("%hd\n", y_2);
	*/

	short newY = (2 * y_1) - y_2 + ((x - (2 * x_6) + x_12) / 32);
	rawLow[arrayStartIndex2] = newY;
	return newY;
}





