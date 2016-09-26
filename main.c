#include <stdlib.h>
#include <stdio.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"
#include "includes/ECGScanner.h"

//TODO make able to start from here

int main(){
	testAll();
	exit(1);

	//TODO Something wrong with the benchmarks. Discuss.
	runBenchmarks();
	exit(1);

	return 0;
}

/*
int main(int argc, char *argv[]){
	runScanner();
	//exit(1);

	testAll();
	exit(1);

	runBenchmarks();
	exit(1);

	return 0;
}
*/
