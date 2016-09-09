#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"

#define RAW_DATA_SIZE_DERIVATIVE 31

short rawSquare[RAW_DATA_SIZE_DERIVATIVE];
short arrayStartIndex4 = 0;

short* getSquareArray()
{
	return rawSquare;
}

short getSqrValue(int offset)
{
	short correctIndex = arrayStartIndex4 + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_DERIVATIVE;
	}
	return rawSquare[correctIndex];
}

void moveArrayStartIndex4()
{
	arrayStartIndex4++;
	if(arrayStartIndex4 == RAW_DATA_SIZE_DERIVATIVE)
	{
		arrayStartIndex4 = 0;
	}
}

short derivativeSquareFilter(int x, int x_1, int x_3, int x_4)
{
	moveArrayStartIndex4();

	short newY = (2 * x + x_1 - x_3 - 2 * x_4) / 8;
	newY = newY * newY;
	rawSquare[arrayStartIndex4] = newY;
	return newY - getSqrValue(-N);
}

