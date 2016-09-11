#include "includes/highPassFilter.h"

#define RAW_DATA_SIZE_HIGH 5

short high_y_4 = 0;
short high_y_3 = 0;
static short high_y_2 = 0;
static short HIGH_Y_1_1 = 0;
short high_y_1 = 0;
static int x_17 = 0;

short highPassFilter(int x, int x_16, int x_32)
{

	short newY = HIGH_Y_1_1 - (x / 32) + x_16 - x_17 + (x_32 / 32);

	x_17 = x_16;

	high_y_4 = high_y_3;
	high_y_3 = high_y_2;
	high_y_2 = high_y_1;
	high_y_1 = HIGH_Y_1_1;
	HIGH_Y_1_1 = newY;

	return newY;
}

void resetHighBuffer()
{
	HIGH_Y_1_1 = 0;
	high_y_1 = 0;
	high_y_2 = 0;
	high_y_3 = 0;
	high_y_4 = 0;
	x_17 = 0;
}






