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

	return 0;
}
