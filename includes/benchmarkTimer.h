/*
 * benchmarkTimer.h
 *
 *  Created on: 9 Sep 2016
 *      Author: feynman
 */

#ifndef BENCHMARKTIMER_H_
#define BENCHMARKTIMER_H_

#include <time.h>

#define BENCHMARK_START (clock())
#define BENCHMARK_TIME(startTime) (clock() - startTime)
#define BENCHMARK_STOP_PRINT(startTime) (printf("Time: %ld ms\n", (BENCHMARK_TIME(startTime)) / 100))
#define BENCHMARK_PRINT_TIME(time) (printf("Time: %ld ms\n", time / 100))

#endif /* BENCHMARKTIMER_H_ */
