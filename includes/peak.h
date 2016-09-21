/*
 * peak.h
 *
 *  Created on: 2016/09/19
 *      Author: jesper
 */

#ifndef INCLUDES_PEAK_H_
#define INCLUDES_PEAK_H_


struct TPeak
{
	const unsigned short intensity;
	unsigned short RR;
	unsigned char refCount;
};

typedef struct TPeak Peak;


#endif /* INCLUDES_PEAK_H_ */
