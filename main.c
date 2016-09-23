#include <stdlib.h>
#include <stdio.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"
#include "includes/ECGScanner.h"

int main(int argc, char *argv[]){
	runScanner();
	exit(1);

	runBenchmarks();
	exit(1);

	return 0;
}
