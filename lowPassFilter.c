#include "includes/lowPassFilter.h"

#define RAW_DATA_SIZE_LOW 33

static int rawLow[RAW_DATA_SIZE_LOW];
static short arrayStartIndex = 0;

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

	int y_1 = getLowPassValue(-1);
	int y_2 = getLowPassValue(-2);

	int newY = (2 * y_1) - y_2 + ((x - (2 * x_6) + x_12) / 32);
	rawLow[arrayStartIndex] = newY;
	return newY;
}





