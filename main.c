#include <stdio.h>
#include <stdlib.h>
#include "includes/inputManager.h"
#include "includes/filter.h"
#include "includes/benchmarks.h"
#include "includes/tests.h"

int main(int argc, char *argv[])
{
	testAll();
	exit(1);
	runBenchmarks();
	exit(1);
	FILE* inputFile = startInputData("ECG.txt"); //max abs 1164

	if(inputFile == NULL)
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

		result += filterData(data);
	}

	/*have to use result for something so just print it*/
	printf("done %ld\n", result);


	stopInputData(inputFile);

	return 0;
}
