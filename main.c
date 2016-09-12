#include <stdio.h>
#include <stdlib.h>
#include "includes/inputManager.h"
#include "includes/filter.h"
<<<<<<< HEAD
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/derivativeSquareFilter.h"
#include "includes/movingWindowFilter.h"
#include "includes/peakSearcher.h"


int main(int argc, char *argv[])
{
	FILE* inputFile = startInputData("ECG.txt"); /*max abs 1164 */
	FILE* x_low = startInputData("verification_files/x_low.txt"); /* max abs 2122988 */
	FILE* x_high = startInputData("verification_files/x_high.txt"); /* max abs 644 */
	FILE* x_sqr = startInputData("verification_files/x_sqr.txt"); /* max abs 21025 */
	FILE* x_mwi = startInputData("verification_files/x_mwi_div_after.txt"); /* max abs 5444 */
	/*testerPeak();*/
	printf("kage\n");
	if(inputFile == NULL ||
	   x_low == NULL ||
	   x_high == NULL ||
	   x_sqr == NULL ||
	   x_mwi == NULL)
	{
		exit(1);
	}

	while(1)
	{
		short data = getNextData(inputFile);
		if(!hasMoreData(inputFile))
		{
			break;
		}
		short filteredData = filterData(data, x_low, x_high, x_sqr, x_mwi);
		searchPeak(filteredData);
		if (hasNewPeak() ){
			isRPeak(getPeakValue(0), getPeakTime(0), getPeakTime(-7));
		}
	}

	printf("done\n");


	stopInputData(inputFile);
	stopInputData(x_low);
	stopInputData(x_high);
	stopInputData(x_sqr);
	stopInputData(x_mwi);
=======
#include "includes/benchmarks.h"
#include "includes/tests.h"

int main(int argc, char *argv[])
{
	testAll();
	exit(1);

	runBenchmarks();
	exit(1);

>>>>>>> d27204af3f1a45d7d4460bcf75c99bc94050ee17
	return 0;
}
