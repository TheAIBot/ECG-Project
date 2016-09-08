#include "includes/derivativeSquareFilter.h"

#define RAW_DATA_SIZE_DERIVATIVE 31

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

short derivativeSquareFilter(short x, short x_1, short x_3, short x_4)
{
	moveArrayStartIndex4();

	short newY = (2 * x + x_1 - x_3 - 2 * x_4) / 8;
	rawSquare[arrayStartIndex4] = newY * newY;
	return newY * newY;
}

