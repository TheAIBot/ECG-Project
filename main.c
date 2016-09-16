#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include <errno.h>
=======
>>>>>>> ec6cd34dafef178b205ce93bee389fe46dac2d61
#include "includes/inputManager.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"
<<<<<<< HEAD
#include "includes/peakSearcher.h"

char checkFilter(short filterOutput, short expectedOutput, char* filterName)
{
	if(filterOutput != expectedOutput)
	{
		printf("wrong output from %s pass filter: %hd != %hd\n", filterName, filterOutput, expectedOutput);
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	testerPeak();
	printf("kage\n");
	/*
=======

int main(int argc, char *argv[])
{
>>>>>>> ec6cd34dafef178b205ce93bee389fe46dac2d61
	FILE* inputFile = startInputData("ECG.txt");
	FILE* x_low = startInputData("verification_files/x_low.txt");
	FILE* x_high = startInputData("verification_files/x_high.txt");
	FILE* x_sqr = startInputData("verification_files/x_sqr.txt");
	FILE* x_mwi = startInputData("verification_files/x_mwi.txt");

<<<<<<< HEAD
	if(inputFile == NULL ||
	   x_low == NULL ||
	   x_high == NULL ||
	   x_sqr == NULL ||
	   x_mwi == NULL)
	{
		exit(1);
	}

	while(1)
	{
		short data = getNextData(inputFile);
		if(!hasMoreData(inputFile))
		{
			break;
		}
		printf("%hd\n", data);
		insertRawData(data);

		short dataLowFiltered = lowPassFilter(data, getRawDataValue(-6), getRawDataValue(-12));
		if(checkFilter(dataLowFiltered, getNextData(x_low), "low"))
		{
=======
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
>>>>>>> ec6cd34dafef178b205ce93bee389fe46dac2d61
			break;
		}

		short dataHighFiltered = highPassFilter(dataLowFiltered, getLowPassValue(-16), getLowPassValue(-17), getLowPassValue(-32));
<<<<<<< HEAD
		if(checkFilter(dataHighFiltered, getNextData(x_high), "high"))
		{
			break;
		}

		short dataDerSqrFiltered = derivativeSquareFilter(dataHighFiltered, getHighPassValue(-1), getHighPassValue(-3), getHighPassValue(-4));
		if(checkFilter(dataDerSqrFiltered, getNextData(x_sqr), "dersqr"))
		{
			break;
		}

		short dataMovingWindowFilter = movingWindowFilter(getSquareArray());
		if(checkFilter(dataMovingWindowFilter, getNextData(x_mwi), "moving window"))
		{
			break;
		}
	}


=======
		derivativeSquareFilter(dataHighFiltered, getHighPassValue(-1), getHighPassValue(-3), getHighPassValue(-4));
		movingWindowFilter(getSquareArray());
	}

>>>>>>> ec6cd34dafef178b205ce93bee389fe46dac2d61
	stopInputData(inputFile);
	stopInputData(x_low);
	stopInputData(x_high);
	stopInputData(x_sqr);
	stopInputData(x_mwi);
<<<<<<< HEAD
	*/
=======

>>>>>>> ec6cd34dafef178b205ce93bee389fe46dac2d61
	return 0;
}
