#ifndef HIGHPASSFILTER_H_
#define HIGHPASSFILTER_H_

short getHighPassValue(int offset);
short highPassFilter(int x, int x_16, int x_17, int x_32);
void resetHighBuffer(void);

#endif /* HIGHPASSFILTER_H_ */
