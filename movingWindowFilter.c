#include "includes/movingWindowFilter.h"

#define RAW_DATA_SIZE_MOVING_WINDOW 13

static short rawMovingWindow[RAW_DATA_SIZE_MOVING_WINDOW];
static short arrayStartIndex = 0;
static int totalValue = 0;

short getMovingWindowValue(int offset)
{
	short correctIndex = arrayStartIndex + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_MOVING_WINDOW;
	}
	return rawMovingWindow[correctIndex];
}

static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE_MOVING_WINDOW)
	{
		arrayStartIndex = 0;
	}
}

short movingWindowFilter(short sqrDiffrence)
{
	moveArrayStartIndex();
	totalValue += sqrDiffrence;

	short newY = totalValue / N;
	rawMovingWindow[arrayStartIndex] = newY;
	return newY;
}

void resetMovingWindowBuffer()
{
	memset(rawMovingWindow, 0, RAW_DATA_SIZE_MOVING_WINDOW * sizeof(short));
	totalValue = 0;
}





