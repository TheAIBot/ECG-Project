#include <string.h>
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"

#define RAW_DATA_SIZE_DERIVATIVE 30

static short rawSquare[RAW_DATA_SIZE_DERIVATIVE];
static short arrayStartIndex = 0;
static short lastValue = 0;

short* getSquareArray()
{
	return rawSquare;
}

short getSqrValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_DERIVATIVE;
	}
	return rawSquare[correctIndex];
}

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE_DERIVATIVE)
	{
		arrayStartIndex = 0;
	}
}

short derivativeSquareFilter(int x, int x_1, int x_3, int x_4)
{
	moveArrayStartIndex();

	short newY = (2 * x + x_1 - x_3 - 2 * x_4) / 8;
	newY = newY * newY;
	lastValue = rawSquare[arrayStartIndex];
	rawSquare[arrayStartIndex] = newY;
	return newY - lastValue;//getSqrValue(-N);
}

void resetSqrBuffer()
{
	memset(rawSquare, 0, RAW_DATA_SIZE_DERIVATIVE * sizeof(short));
	lastValue = 0;
}

