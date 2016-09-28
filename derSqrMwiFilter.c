#include "includes/derSqrMwiFilter.h"

#include <string.h>

#define RAW_DATA_SIZE_DERIVATIVE 30
#define N 30

static unsigned short rawSquare[RAW_DATA_SIZE_DERIVATIVE];
static short arrayStartIndex = 0;
static unsigned int totalValue = 0;

//moves the start index of the circular array rawSquare forward.
//with looping when the end of the array has been passed.
static void moveArrayStartIndex()
{
	arrayStartIndex++;
	if(arrayStartIndex == RAW_DATA_SIZE_DERIVATIVE)
	{
		arrayStartIndex = 0;
	}
}

unsigned short derivativeSquareMovingWindowFilter(const short x, const short x_1, const short x_3, const short x_4)
{
	moveArrayStartIndex();

	//derivate filter equation
	unsigned short newY = (2 * x + x_1 - x_3 - 2 * x_4) / 8;
	//unsigned int newY = (2 * x + x_1 - x_3 - 2 * x_4);

	//square filter equation
	unsigned short squared = newY * newY;

	//The sum of an array can be calculated with the equation (totalSum + newValue - OldValue) / Length.
	//totalSum is zero to begin with.
	//oldValue is equal to the value at the index that the new value will occupy.
	unsigned short lastSquaredValue = rawSquare[arrayStartIndex];
	rawSquare[arrayStartIndex] = squared;

	//moving window filter equation
	totalValue += squared - lastSquaredValue;
	return totalValue / N;
	//return totalValue / (N * 8 * 8);
}

//sets all values in the cirkular array to 0
//and sets all cached values to 0.
void resetDerSqrMwiFilter()
{
	memset(rawSquare, 0, RAW_DATA_SIZE_DERIVATIVE * sizeof(unsigned short));
	totalValue = 0;
}

