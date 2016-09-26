#include <string.h>
#include "includes/lowPassFilter.h"

#define RAW_DATA_SIZE_LOW 32

static int rawLow[RAW_DATA_SIZE_LOW];
static short arrayStartIndex = 0;
static int y_1 = 0;
static int y_2 = 0;
int low_y_32 = 0;

int getLowPassValue(const int offset){
	return rawLow[(arrayStartIndex + offset) & 0x1F];
}

static void moveArrayStartIndex(){
	arrayStartIndex++;
	//when arrayStartindex == RAW_DATA_SIZE_LOW the circular array start index has to begin
	//at 0 again and because the length of the array is a power of two it's possible to do just that
	//with a bitwise and which sets all the bits above the fifth bit to zero zero.
	arrayStartIndex &= 0x1F;
}

int lowPassFilter(const int x, const int x_6, const int x_12){
	moveArrayStartIndex();
	/*
	printf("%d\n", y_1);
	printf("%d\n", y_2);
	printf("%d\n", x);
	printf("%d\n", x_6);
	printf("%d\n", x_12);
	*/
	int newY = (2 * y_1) - y_2 + ((x - (2 * x_6) + x_12) / 32);
	low_y_32 = rawLow[arrayStartIndex];
	rawLow[arrayStartIndex] = newY;

	y_2 = y_1;
	y_1 = newY;
	return newY;
}

void resetLowFilter(){
	memset(rawLow, 0, RAW_DATA_SIZE_LOW * sizeof(int));
	y_1 = 0;
	y_2 = 0;
	low_y_32 = 0;
}





