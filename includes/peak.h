/*
 * peak.h
 *
 *  Created on: 2016/09/19
 *      Author: jesper
 */

#ifndef INCLUDES_PEAK_H_
#define INCLUDES_PEAK_H_

/* A datastructure representing a peak. Contains two values, the intensity (intensity) at the time the peak was recorded,
 * and the time since the last true R-peak found (the RR value)*/
struct TPeak{
	unsigned short intensity;
	unsigned short RR;
};

typedef struct TPeak Peak;


#endif /* INCLUDES_PEAK_H_ */
