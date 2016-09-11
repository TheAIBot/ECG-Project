#ifndef HIGHPASSFILTER_H_
#define HIGHPASSFILTER_H_

extern short high_y_4;
extern short high_y_3;
extern short high_y_1;

short getHighPassValue(int offset);
short highPassFilter(int x, int x_16, int x_32);
void resetHighBuffer(void);

#endif /* HIGHPASSFILTER_H_ */
