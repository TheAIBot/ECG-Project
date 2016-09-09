#include <stdio.h>
#include <stdlib.h>
#include "includes/inputManager.h"
#include "includes/filter.h"
#include "includes/benchmarks.h"

int main(int argc, char *argv[])
{
	benchmarkAll();
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
		exit(-1);
	}

	/*this variable is used to gather the output of filterData so the compiler can't optimize the
	 * method call out of the while loop*/
	long result = 0;
	while(1)
	{
		short data = getNextData(inputFile);
		if(!hasMoreData(inputFile))
		{
			break;
		}

		result += filterData(data, x_low, x_high, x_sqr, x_mwi);
	}

	/*have to use result for something so just print it*/
	printf("done %ld\n", result);


	stopInputData(inputFile);
	stopInputData(x_low);
	stopInputData(x_high);
	stopInputData(x_sqr);
	stopInputData(x_mwi);

	return 0;
}
