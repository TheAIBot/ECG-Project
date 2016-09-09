#include "includes/derivativeSquareFilter.h"

#define RAW_DATA_SIZE_DERIVATIVE 30

short rawSquare[RAW_DATA_SIZE_DERIVATIVE];
short arrayStartIndex4 = 0;

short* getSquareArray()
{
	return rawSquare;
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
	return newY;
}

