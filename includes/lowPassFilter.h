/*Made by the glorious Andreas*/

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

#define GET_LOW_FILTER_VALUE_X_32 low_y_32

extern int low_y_32;

int getLowPassValue(const int offset);
int lowPassFilter(const int x, const int x_6, const int x_12);
void resetLowFilter(void);

#endif /* LOWPASSFILTER_H_ */
