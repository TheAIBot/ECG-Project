/*Made by the glorious Andreas*/

#ifndef BENCHMARKTIMER_H_
#define BENCHMARKTIMER_H_

#include <time.h>

#define BENCHMARK_START (clock())
#define BENCHMARK_TIME(startTime) (clock() - startTime)
#define BENCHMARK_STOP_PRINT(startTime) (BENCHMARK_PRINT_TIME(BENCHMARK_TIME(startTime)))
#define BENCHMARK_PRINT_TIME(time) (printf("Time: %ld ms\n", time / 1000))

#endif /* BENCHMARKTIMER_H_ */
