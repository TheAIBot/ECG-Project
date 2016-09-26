#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"
#include "includes/ECGScanner.h"

int main(int argc, char *argv[])
{
	//if no arguments is given then just run
	//the program normally
	if(argc == 1)
	{
		runScanner("ECG.txt");
	}
	else if(argc == 2 ||
			argc == 3)
	{
		if(strcmp(argv[1], "--test") == 0)
		{
			testAll();
		}
		else if(strcmp(argv[1], "--benchmark") == 0)
		{
			runBenchmarks();
		}
		else if(strcmp(argv[1], "--file") == 0 &&
				argc == 3)
		{
			runScanner(argv[2]);
		}
		else
		{
			printf("Possible arguments are\n  --test\n  --benchmark\n  --file <filepath>\n");
		}
	}
	else
	{
		printf("Too many arguments given");
	}

	return 0;
}
