#include "includes/highPassFilter.h"

#define RAW_DATA_SIZE_HIGH 5

static short rawHigh[RAW_DATA_SIZE_HIGH];
static short arrayStartIndex = 0;

short getHighPassValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_HIGH;
	}
	return rawHigh[correctIndex];
}

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE_HIGH)
	{
		arrayStartIndex = 0;
	}
}

short highPassFilter(int x, int x_16, int x_17, int x_32)
{
	moveArrayStartIndex();

	short y_1 = getHighPassValue(-1);
	//printf("%hd\n", y_1);
	//printf("%d\n", x);
	//printf("%d\n", x_16);
	//printf("%d\n", x_17);
	//printf("%d\n", x_32);

	short newY = y_1 - (x / 32) + x_16 - x_17 + (x_32 / 32);
	rawHigh[arrayStartIndex] = newY;
	return newY;
}





