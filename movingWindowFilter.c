#include "includes/movingWindowFilter.h"

#define RAW_DATA_SIZE_MOVING_WINDOW 13

short rawMovingWindow[RAW_DATA_SIZE_MOVING_WINDOW];
short arrayStartIndex5 = 0;

short getMovingWindowValue(int offset)
{
	short correctIndex = arrayStartIndex5 + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_MOVING_WINDOW;
	}
	return rawMovingWindow[correctIndex];
}

void moveArrayStartIndex5()
{
	arrayStartIndex5++;
	if(arrayStartIndex5 == RAW_DATA_SIZE_MOVING_WINDOW)
	{
		arrayStartIndex5 = 0;
	}
}

short movingWindowFilter(short sqrDiffrence)
{
	moveArrayStartIndex5();
	static int totalValue = 0;
	totalValue += sqrDiffrence;

	short newY = totalValue / N;
	rawMovingWindow[arrayStartIndex5] = newY;
	return newY;
}





