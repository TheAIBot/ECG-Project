#include "includes/movingWindowFilter.h"

#define RAW_DATA_SIZE_MOVING_WINDOW 13
#define N 30

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

short movingWindowFilter(short x[N])
{
	moveArrayStartIndex5();

	int newY = 0;
	int i = 0;
	for(i = 0; i < N; i++)
	{
		newY += x[i];
	}
	newY /= N;
	rawMovingWindow[arrayStartIndex5] = (short)newY;
	return newY;
}





