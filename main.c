#include <stdio.h>
#include "includes/inputManager.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"

int main(int argc, char *argv[])
{
	printf("kage\n");
	FILE* inputFile = startInputData("ECG.txt");
	FILE* x_low = startInputData("verification_files/x_low.txt");
	FILE* x_high = startInputData("verification_files/x_high.txt");
	FILE* x_sqr = startInputData("verification_files/x_sqr.txt");
	FILE* x_mwi = startInputData("verification_files/x_mwi.txt");

	while(hasMoreData(inputFile))
	{
		short data = getNextData(inputFile);
		printf("%d\n", data);
		insertRawData(data);

		short dataLowFiltered = lowPassFilter(data, getRawDataValue(-6), getRawDataValue(-12));
		short expectedLowFilteredData = getNextData(x_low);
		if(dataLowFiltered != expectedLowFilteredData);
		{
			printf("wrong output of low pass filter: %d != %d\n", dataLowFiltered, expectedLowFilteredData);
			break;
		}

		short dataHighFiltered = highPassFilter(dataLowFiltered, getLowPassValue(-16), getLowPassValue(-17), getLowPassValue(-32));
		derivativeSquareFilter(dataHighFiltered, getHighPassValue(-1), getHighPassValue(-3), getHighPassValue(-4));
		movingWindowFilter(getSquareArray());
	}

	stopInputData(inputFile);
	stopInputData(x_low);
	stopInputData(x_high);
	stopInputData(x_sqr);
	stopInputData(x_mwi);

	return 0;
}
