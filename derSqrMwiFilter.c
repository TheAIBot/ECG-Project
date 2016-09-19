#include "includes/derSqrMwiFilter.h"

#include <string.h>

#define RAW_DATA_SIZE_DERIVATIVE 30
#define N 30

static unsigned short rawSquare[RAW_DATA_SIZE_DERIVATIVE];
static short arrayStartIndex = 0;
static unsigned int totalValue = 0;

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE_DERIVATIVE)
	{
		arrayStartIndex = 0;
	}
}

unsigned short derivativeSquareMovingWindowFilter(int x, int x_1, int x_3, int x_4)
{
	moveArrayStartIndex();

	//derivate filter
	unsigned short newY = (2 * x + x_1 - x_3 - 2 * x_4) / 8;

	//square filter
	unsigned short squared = newY * newY;

	unsigned short lastSquaredValue = rawSquare[arrayStartIndex];
	rawSquare[arrayStartIndex] = squared;

	//moving window filter
	totalValue += squared - lastSquaredValue;
	return totalValue / N;
}

void resetSqrBuffer()
{
	memset(rawSquare, 0, RAW_DATA_SIZE_DERIVATIVE * sizeof(short));
	totalValue = 0;
}

