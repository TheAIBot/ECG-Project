#include "includes/lowPassFilter.h"

#define RAW_DATA_SIZE_LOW 33

static int rawLow[RAW_DATA_SIZE_LOW];
static short arrayStartIndex = 0;
static int y_1 = 0;
static int y_2 = 0;

int getLowPassValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_LOW;
	}
	return rawLow[correctIndex];
}

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE_LOW)
	{
		arrayStartIndex = 0;
	}
}

int lowPassFilter(int x, int x_6, int x_12)
{
	moveArrayStartIndex();
	/*
	printf("%d\n", y_1);
	printf("%d\n", y_2);
	printf("%d\n", x);
	printf("%d\n", x_6);
	printf("%d\n", x_12);
	*/

	int newY = (2 * y_1) - y_2 + ((x - (2 * x_6) + x_12) / 32);
	rawLow[arrayStartIndex] = newY;

	y_2 = y_1;
	y_1 = newY;

	return newY;
}

void resetLowBuffer()
{
	memset(rawLow, 0, RAW_DATA_SIZE_LOW * sizeof(int));
	y_1 = 0;
	y_2 = 0;
}





