#include <stdlib.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"


int main(int argc, char *argv[])
{
	initializeRPeakFinder();
	testAll();
	//exit(1);
	runBenchmarks();
	exit(1);
	return 0;
}
