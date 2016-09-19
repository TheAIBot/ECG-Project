#include <stdlib.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"

int main(int argc, char *argv[])
{
	testSearchBackDoubleSearchTest();
	testAll();
	//exit(1);

	runBenchmarks();
	exit(1);
	return 0;
}
