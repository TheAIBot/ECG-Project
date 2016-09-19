#ifndef HIGHPASSFILTER_H_
#define HIGHPASSFILTER_H_

#define GET_HIGH_PASS_VALUE(index) (rawHigh[index])

#define RAW_DATA_SIZE_HIGH 5

#define X_1_INDEX 1
#define X_3_INDEX 3
#define X_4_INDEX 4


extern short rawHigh[RAW_DATA_SIZE_HIGH];

short highPassFilter(const int x, const int x_16, const int x_32);
void resetHighFilter(void);

#endif /* HIGHPASSFILTER_H_ */
