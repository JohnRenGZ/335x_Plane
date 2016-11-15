/*
 * CBMP280.h
 *
 *  Created on: Nov 15, 2016
 *      Author: ryj
 */

#ifndef DRIVERS_BMP280_CBMP280_H_
#define DRIVERS_BMP280_CBMP280_H_

#include <stdint.h>

#include "../common/Barometer/CBarometer.h"

class CBMP280 : public CBarometer {
public:
	CBMP280();
	virtual ~CBMP280();

	int Init(void);
	int GetTemperature(float &temperature);

private:
	int m_devHandle;

	int writeReg(int pt, unsigned char add, char value);
	int readReg(int pt, unsigned char s_add, char buf[], int size);
	float caleTemp(uint32_t adc_T, unsigned short dig_T1, short dig_T2, short dig_T3);
};

#endif /* DRIVERS_BMP280_CBMP280_H_ */
