/*Made by the glorious Andreas*/

#ifndef DERIVATIVESQUAREFILTER_H_
#define DERIVATIVESQUAREFILTER_H_

unsigned short derivativeSquareMovingWindowFilter(const short x, const short x_1, const short x_3, const short x_4);
void resetDerSqrMwiFilter(void);

#endif /* DERIVATIVESQUAREFILTER_H_ */
