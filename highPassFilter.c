#include "includes/highPassFilter.h"

<<<<<<< HEAD
#define RAW_DATA_SIZE_HIGH 5
=======
#define RAW_DATA_SIZE_HIGH 4
>>>>>>> ec6cd34dafef178b205ce93bee389fe46dac2d61

short rawHigh[RAW_DATA_SIZE_HIGH];
short arrayStartIndex3 = 0;

short getHighPassValue(int offset)
{
	short correctIndex = arrayStartIndex3 + offset;
	if(correctIndex < 0)
	{
		correctIndex += RAW_DATA_SIZE_HIGH;
	}
	return rawHigh[correctIndex];
}

void moveArrayStartIndex3()
{
	arrayStartIndex3++;
	if(arrayStartIndex3 == RAW_DATA_SIZE_HIGH)
	{
		arrayStartIndex3 = 0;
	}
}

short highPassFilter(short x, short x_16, short x_17, short x_32)
{
	moveArrayStartIndex3();

	short y_1 = getHighPassValue(-1);

	short newY = y_1 - (x / 32) + x_16 - x_17 + (x_32 / 32);
	rawHigh[arrayStartIndex3] = newY;
	return newY;
}





