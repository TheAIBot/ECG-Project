#include "includes/derSqrMwiFilter.h"

#include <string.h>

#define RAW_DATA_SIZE_DERIVATIVE 30
#define N 30

static short rawSquare[RAW_DATA_SIZE_DERIVATIVE];
static short arrayStartIndex = 0;
static int totalValue = 0;

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

short derivativeSquareMovingWindowFilter(int x, int x_1, int x_3, int x_4)
{
	moveArrayStartIndex();

	//derivate filter
	short newY = (2 * x + x_1 - x_3 - 2 * x_4) / 8;

	//square filter
	newY = newY * newY;

	short lastValue = rawSquare[arrayStartIndex];
	rawSquare[arrayStartIndex] = newY;

	//moving window filter
	newY = newY - lastValue;
	totalValue += newY;
	return totalValue / N;
}

void resetSqrBuffer()
{
	memset(rawSquare, 0, RAW_DATA_SIZE_DERIVATIVE * sizeof(short));
}

