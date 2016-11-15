/*
 * CBarometer.h
 *
 *  Created on: Nov 15, 2016
 *      Author: ryj
 */

#ifndef DRIVERS_COMMON_BAROMETER_CBAROMETER_H_
#define DRIVERS_COMMON_BAROMETER_CBAROMETER_H_

class CBarometer {
public:
	CBarometer();
	virtual ~CBarometer();

	virtual int Init(void) = 0;
	virtual int GetTemperature(float &temperature) = 0;
};

#endif /* DRIVERS_COMMON_BAROMETER_CBAROMETER_H_ */
