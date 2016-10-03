#include <stdio.h>
#include <stdlib.h>
#include "includes/tests.h"
#include "includes/circularArray.h"
#include "includes/derSqrMwiFilter.h"
#include "includes/rawData.h"
#include "includes/lowPassFilter.h"
#include "includes/highPassFilter.h"
#include "includes/filter.h"
#include "includes/rPeakFinder.h"
#include "includes/peak.h"
#include "includes/peakSearcher.h"
#include "includes/sensor.h"
#include "includes/peakCircularArray.h"


/*
 * Without these tests the program would crash and burn everytime the program is compiled.
 * Unfortunatly the price of no bugs is high and many chips and drinks has been sacrificed to the compiler gods.
 * If you encounter any bugs then a sacrifice to the compiler gods should make it all go away(works 100% of the time 50% of the time).
 */



#define TEST_DATA_LENGTH 10000 /* 10.000 */
#define TEST_DATA_R_PEAK_LENGTH 31
#define ALLOWED_TIME_DEVIANTION 10
#define ALLOWED_MEASUREMENT_DEVIATION 300
#define ALLOWED_FILTER_DEVIATION 0

char isFilterCorrect(int filterOutput, FILE* file, char* filterName)
{
	if(file != NULL)
	{
		int expectedOutput = getNextData(file);
		if(expectedOutput + ALLOWED_FILTER_DEVIATION < filterOutput ||
		   expectedOutput - ALLOWED_FILTER_DEVIATION > filterOutput)
		{
			fprintf(stderr, "wrong output from %s filter: Given %hd != %hd Expected\n", filterName, filterOutput, expectedOutput);
			return 0;
		}
	}
	return 1;
}

void flushFilterBuffers()
{
	resetRawBuffer();
	resetLowFilter();
	resetHighFilter();
	resetDerSqrMwiFilter();
}

char testLowPassFilter(int* data){
	resetLowFilter();
	FILE* file = startInputData("verification_files/x_low.txt");
	FILE* writeFile = fopen("test_results/low_results.txt", "w");
	if(file == NULL || writeFile == NULL)	{
		return 0;
	}

	CircularArray circArray;
	if(!initCircArray(&circArray, 16, 0))	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)	{
		insertCircArrayData(&circArray, data[i]);
		int dataLowFiltered = lowPassFilter(data[i], getCircArrayValue(&circArray, -6), getCircArrayValue(&circArray, -12));
		if(!isFilterCorrect(dataLowFiltered, file, "low"))		{
			stopInputData(file);
			freeCircArray(&circArray);
			fclose(writeFile);
			return 0;
		}
		fprintf(writeFile, "%d\n", dataLowFiltered);
	}
	stopInputData(file);
	freeCircArray(&circArray);
	fclose(writeFile);
	printf("Passed low pass filter test\n");
	return 1;
}

char testHighPassFilter(int* data)
{
	resetHighFilter();
	FILE* file = startInputData("verification_files/x_high.txt");
	FILE* writeFile = fopen("test_results/high_results.txt", "w");
	if(file == NULL ||
	   writeFile == NULL)
	{
		return 0;
	}

	CircularArray circArray;
	if(!initCircArray(&circArray, 64, 0))
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertCircArrayData(&circArray, data[i]);

		int dataLowFiltered = highPassFilter(data[i], getCircArrayValue(&circArray, -16), getCircArrayValue(&circArray, -32));
		if(!isFilterCorrect(dataLowFiltered, file, "high"))
		{
			stopInputData(file);
			freeCircArray(&circArray);
			fclose(writeFile);
			return 0;
		}
		fprintf(writeFile, "%d\n", dataLowFiltered);
	}
	stopInputData(file);
	freeCircArray(&circArray);
	fclose(writeFile);
	printf("Passed high pass filter test\n");
	return 1;
}

char testDerivSqrFilter(int* data)
{
	resetDerSqrMwiFilter();
	FILE* file = startInputData("verification_files/x_mwi_div_after.txt");
	FILE* writeFile = fopen("test_results/dersqrmwi_results.txt", "w");
	if(file == NULL ||
	   writeFile == NULL)
	{
		return 0;
	}

	CircularArray circArray;
	if(!initCircArray(&circArray, 8, 0))
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{
		insertCircArrayData(&circArray, data[i]);

		short mwiFiltered = derivativeSquareMovingWindowFilter(data[i], getCircArrayValue(&circArray, -1), getCircArrayValue(&circArray, -3), getCircArrayValue(&circArray, -4));
		if(!isFilterCorrect(mwiFiltered, file, "derivative square moving window"))
		{
			stopInputData(file);
			freeCircArray(&circArray);
			fclose(writeFile);
			return 0;
		}
		fprintf(writeFile, "%d\n", mwiFiltered);
	}
	stopInputData(file);
	freeCircArray(&circArray);
	fclose(writeFile);
	printf("Passed derivative square moving window filter test\n");
	return 1;
}

char testWholeFilter(int* data){
	flushFilterBuffers();
	FILE* file = startInputData("verification_files/x_mwi_div_after.txt");
	FILE* writeFile = fopen("test_results/wholefilter_results.txt", "w");
	if(file == NULL ||
	   writeFile == NULL)
	{
		return 0;
	}

	for(int i = 0; i < TEST_DATA_LENGTH; i++)
	{

		int dataLowFiltered = filterData(data[i]);
		if(!isFilterCorrect(dataLowFiltered, file, "whole"))
		{
			stopInputData(file);
			fclose(writeFile);
			return 0;
		}
		fprintf(writeFile, "%d\n", dataLowFiltered);

	}
	stopInputData(file);
	fclose(writeFile);
	printf("Passed whole filter test\n");
	return 1;
}

char testRPeakSearcher(int* data){
	flushFilterBuffers();
	FILE* file = startInputData("verification_files/correct_Rpeak.txt");
	FILE* writeFile = fopen("test_results/rpeaks_results.txt", "w");
	FILE* write = fopen("test_results/thresholds.txt", "w");
	FILE* writePeaks = fopen("test_results/peaks.txt", "w");
	if(file == NULL || writeFile == NULL || write == NULL || writePeaks == NULL)	{
		return 0;
	}

	int* timesAndMeasurements = loadPeakData(file, TEST_DATA_R_PEAK_LENGTH);
	int* times = timesAndMeasurements;
	int* measurements = &timesAndMeasurements[TEST_DATA_R_PEAK_LENGTH];
	char timeMeasurementTaken[TEST_DATA_R_PEAK_LENGTH] = {0};
	int timeSum = 0;
	//The r-peak finder has already been initialized.
	PeakAvgCircularArray* trueRRPeaks = getTrueRPeaksArray();
	//printf("started\n");
	for(int i = 0; i < TEST_DATA_LENGTH; i++){
		if(foundPeak(data[i]))	{
			fprintf(writePeaks, "%d\n", i);
			Peak newPeak = getNewPeak();
			if (isRPeak(newPeak)){
				//setFoundNewRRPeak(); Not needed anymore
				int newRPeakCount = getNewRPeaksFoundCount();
				for(int y = 0; y < newRPeakCount; y++){
					Peak newRRPeak = getPeakAvgCircValue(trueRRPeaks, -((newRPeakCount - 1) - y));
					char isCorrect = 0;
					for(int z = 0; z < TEST_DATA_R_PEAK_LENGTH; z++){
						if(timeMeasurementTaken[z] == 0 &&
						   times[z] + ALLOWED_TIME_DEVIANTION >= newRRPeak.RR + timeSum &&
						   times[z] - ALLOWED_TIME_DEVIANTION <= newRRPeak.RR + timeSum &&
						   measurements[z] + ALLOWED_MEASUREMENT_DEVIATION >= newRRPeak.intensity &&
						   measurements[z] - ALLOWED_MEASUREMENT_DEVIATION <= newRRPeak.intensity)
						{
							timeMeasurementTaken[z] = 1;
							isCorrect = 1;
							int currentTime = newRRPeak.RR + timeSum;
							fprintf(writeFile, "%d %d\n", currentTime, newRRPeak.intensity);
							break;
						}
					}
					if(!isCorrect){
						printf("Failed to find matching peak for time: %d, measurement: %d\n", newRRPeak.RR + timeSum, newRRPeak.intensity);
						free(timesAndMeasurements);
						fclose(writeFile);
						fclose(write);
						fclose(writePeaks);
						return 0;
					}
					timeSum += newRRPeak.RR;
				}
			}
		}
		fprintf(write, "%d %d\n", Threshold1, Threshold2);
	}
	char foundAll = 1;
	for(int i = 0; i < TEST_DATA_R_PEAK_LENGTH; i++)	{
		if(timeMeasurementTaken[i] == 0)		{
			foundAll = 0;
			printf("Couldn't find a match for time: %d, value: %d\n", times[i], measurements[i]);
			break;
		}
	}
	if(getTimeSinceLastRPeakFound() != 	TEST_DATA_LENGTH - timeSum - 2){
		printf("Wrong timeSinceLastRPeakFound in the peak finder. \n");
		printf("Should be the difference between the lenght of the file, and the sum of all RR values, minus 2, %d, but is %d \n", TEST_DATA_LENGTH - timeSum, getTimeSinceLastRPeakFound());
	}
	setTimeSinceLastRPeakFound(0);


	if(!foundAll)	{
		printf("Failed to find all peaks\n");
		free(timesAndMeasurements);
		fclose(writeFile);
		fclose(write);
		fclose(writePeaks);
		return 0;
	} else 	{
		printf("Passed r peak searcher test\n");
		free(timesAndMeasurements);
		fclose(writeFile);
		fclose(write);
		fclose(writePeaks);
		return 1;
	}
}

char testSearchback(int* data){
	FILE* file = startInputData("verification_files/correct_Rpeak.txt");
	if(file == NULL)	{
		return 0;
	}
	resetRPeakFinder();
	PeakAvgCircularArray* trueRRPeaks = getTrueRPeaksArray();
	for(int i = 0; i < 350; i++){
		if(foundPeak(data[i]))	{
			Peak newPeak = getNewPeak();
			if (isRPeak(newPeak)){
				if (i != 342 - 1){
					printf("Detected a peak at the wrong time, should have been at place 341 instead of %d \n", i);
					return 0;
				} else {
					if((getPeakAvgCircValue(trueRRPeaks, 0).RR == 50 &&
							 getPeakAvgCircValue(trueRRPeaks, 0).intensity == 800) &&
							(getPeakAvgCircValue(trueRRPeaks, -1).RR == 80 &&
							 getPeakAvgCircValue(trueRRPeaks, -1).intensity == 1000)){
						//printf("The two peaks searched for, has been found\n");
						if(getNewRPeaksFoundCount() == 2){
							//printf("The number of peaks found is also correct \n");
						} else {
							printf("FAIL!!! The number of peaks found is not correct \n");
							stopInputData(file);
							return 0;
						}
					} else{
						printf("FAIL!!! The peaks supposed to be found by the searchback, \n is not the ones found.\n");
						stopInputData(file);
						return 0;
					}
				}
				stopInputData(file);
				printf("Passed searchback test. The super edge case for the searchback worked! \n");
				return 1;
			}
		}
	}
	stopInputData(file);
	printf("Something went wrong in the searchback test. \n");
	return 1;
}

void testAll(){
	initializeRPeakFinder();

	int* ecgData = loadDataArray("ECG.txt", TEST_DATA_LENGTH);
	if(ecgData == NULL || !testLowPassFilter(ecgData))	{
		printf("FAIL!!! Didn't pass lowpass filter test\n");
		return;
	}
	/*ecgData is freed at the bottom*/

	int* lowData = loadDataArray("verification_files/x_low.txt", TEST_DATA_LENGTH);
	if(lowData == NULL || !testHighPassFilter(lowData))	{
		printf("FAIL!!! Didn't pass highpass filter test\n");
		free(lowData);
		return;
	}
	free(lowData);

	int* highData = loadDataArray("verification_files/x_high.txt", TEST_DATA_LENGTH);
		if(highData == NULL || !testDerivSqrFilter(highData))		{
			printf("FAIL!!! Didn't pass DerivSqrFilter filter test\n");
			free(highData);
			return;
	}
	free(highData);

	if(ecgData == NULL || !testWholeFilter(ecgData))	{
		printf("FAIL!!! Didn't pass whole filter test\n");
		free(ecgData);
		return;
	}
	free(ecgData);



	int* mwi_after = loadDataArray("verification_files/x_mwi_div_after.txt", TEST_DATA_LENGTH);
	if(mwi_after == NULL || !testRPeakSearcher(mwi_after)){
		printf("FAIL!!! Didn't pass R-peak searcher test\n");
		free(mwi_after);
		return;
	}
	free(mwi_after);



	int* searchbackTestData = loadDataArray("searchbackTest.txt", 350);
	if(searchbackTestData == NULL || !testSearchback(searchbackTestData)){
		printf("FAIL!!! Didn't pass searchback test\n");
		free(searchbackTestData);
		return;
	}
	free(searchbackTestData);

	printf("All tests finished successfully\n");
}






