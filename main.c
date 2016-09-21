#include <stdlib.h>
#include <stdio.h>
#include "includes/benchmarks.h"
#include "includes/tests.h"
#include "includes/peakSearcher.h"

void testerPeak()
{
	int kage[] = { 20, 19, 18, 23, 22, 26, 18, 2, 50, 60, 60, 20, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
	for (int i = 0; i < 21; i++)
	{
		printf("for run number %d \n", i);
		Peak* newPeak = getIfPeak(kage[i]);
		if(newPeak != NULL)
		{
			printf("susccess\n");
			printf("%p\n", &newPeak);
			unsigned short intensity = newPeak->intensity;
			printf("success\n");
			printf("Found peak: %d\n", intensity);
		}
	}
	printf("done\n");
}


int main(int argc, char *argv[])
{
	testerPeak();
	exit(1);

	//initializeRPeakFinder();

	//testAll();
	//exit(1);

	//runBenchmarks();
	//exit(1);

	return 0;
}
