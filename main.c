#include <stdio.h>
#include <stdlib.h>
#include "includes/inputManager.h"
#include "includes/filter.h"
#include "includes/benchmarkTimer.h"

int main(int argc, char *argv[])
{
	FILE* inputFile = startInputData("ECG.txt"); //max abs 1164
	FILE* x_low = startInputData("verification_files/x_low.txt"); // max abs 2122988
	FILE* x_high = startInputData("verification_files/x_high.txt"); // max abs 644
	FILE* x_sqr = startInputData("verification_files/x_sqr.txt"); // max abs 21025
	FILE* x_mwi = startInputData("verification_files/x_mwi_div_after.txt"); // max abs 5444

	if(inputFile == NULL ||
	   x_low == NULL ||
	   x_high == NULL ||
	   x_sqr == NULL ||
	   x_mwi == NULL)
	{
		exit(1);
	}

	short inputArray[10000];
	for(int i = 0; i < 10000; i++)
	{
		inputArray[i] = getNextData(inputFile);
	}


	long startTime = BENCHMARK_START;
	long result = 0;
	int i = 0;
	while(i < 10000)
	{
		/*short data = getNextData(inputFile);
		if(!hasMoreData(inputFile))
		{
			break;
		}
		*/
		short data = inputArray[i];
		//short filteredData = filterData(data, x_low, x_high, x_sqr, x_mwi);
		result += filterData(data, NULL, NULL, NULL, NULL);
		//printf("%d\n", filteredData);
		i++;
	}
	BENCHMARK_STOP_PRINT(startTime);

	printf("%ld\n", result);
	printf("done\n");


	stopInputData(inputFile);
	stopInputData(x_low);
	stopInputData(x_high);
	stopInputData(x_sqr);
	stopInputData(x_mwi);

	return 0;
}
