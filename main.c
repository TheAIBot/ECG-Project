#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"
#include "includes/ECGScanner.h"

int main(int argc, char *argv[]) {
	//if no arguments is given then just run
	//the program normally

	printf("%d\n", argc);
	switch(argc)
	{
		case 1:
			runScanner("ECG.txt", 1);
			break;
		case 2:
			if (strcmp(argv[1], "--test") == 0) {
				testAll();
			} else if (strcmp(argv[1], "--benchmark") == 0) {
				runBenchmarks();
			} else {
				printf("Possible arguments are\n  --test\n  --benchmark\n  --file <filepath>\n");
			}
			break;
		case 3:
		case 4:
			if (strcmp(argv[1], "--file") == 0) {
				char showUI = 1;
				if(argc == 4)
				{
					showUI = (strcmp(argv[3], "true") == 0);
				}
				runScanner(argv[2], showUI);
			}
			break;
		default:
			printf("Too many arguments given\n");
			break;
	}

	return 0;
}

