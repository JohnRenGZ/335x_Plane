/*
 * CBMP280.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: ryj
 */

#include "CBMP280.h"

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define CBMP280_DEV_ADD 0x76
#define CBMP280_DIG_START 0x88
#define CBMP280_TEMP_START 0xFA
#define CBMP280_CTRL_MEAS 0xF4
#define CBMP280_TEMP_ONLY_NORMAL_MODE 0xE3 // 111 000 11

CBMP280::CBMP280() {
	// TODO Auto-generated constructor stub
	m_devHandle = 0;

}

CBMP280::~CBMP280() {
	// TODO Auto-generated destructor stub
	if (m_devHandle != 0) {
		close(m_devHandle);
		m_devHandle = 0;
	}
}

int CBMP280::Init(void) {
	if (m_devHandle != 0) {
		close(m_devHandle);
		m_devHandle = 0;
	}

	if ((m_devHandle = open("/dev/i2c-1", O_RDWR)) < 0) {
		perror("failed to open the bus\n");
		return (1);
	}
	if (ioctl(m_devHandle, I2C_SLAVE, CBMP280_DEV_ADD) < 0) {
		perror("Failed to connect to the sensor\n");
		return (1);
	}

	if (writeReg(m_devHandle, CBMP280_CTRL_MEAS, CBMP280_TEMP_ONLY_NORMAL_MODE) == 1) {
		return (1);
	}

	return (0);
}

int CBMP280::GetTemperature(float &temperature) {
	char dig_buff[6];
	char tmp_buff[3];

	if (readReg(m_devHandle, CBMP280_DIG_START, dig_buff, 6) == 1) {
		return (1);
	}
	if (readReg(m_devHandle, CBMP280_TEMP_START, tmp_buff, 3) == 1) {
		return (1);
	}

	int adc_T = ((tmp_buff[0] << 16) | (tmp_buff[1] << 8) | (tmp_buff[2])) >> 4;
	unsigned short dig_T1 = (dig_buff[1] << 8) | (dig_buff[0]);
	short dig_T2 = (dig_buff[3] << 8) | (dig_buff[2]);
	short dig_T3 = (dig_buff[5] << 8) | (dig_buff[4]);

	temperature = caleTemp(adc_T, dig_T1, dig_T2, dig_T3);

	return (0);
}

int CBMP280::writeReg(int pt, unsigned char add, char value) {
	unsigned char w_buff[2];
	w_buff[0] = add;
	w_buff[1] = value;
	if (write(pt, w_buff, 2) != 2) {
		perror("Failed write to device");
		return 1;
	}
	return 0;
}
int CBMP280::readReg(int pt, unsigned char s_add, char buf[], int size) {
	char writeBuff[1] = { s_add };
	if (write(pt, writeBuff, 1) != 1) {
		perror("Failed to reset the read address\n");
		return 1;
	}
	if (read(pt, buf, size) != size) {
		perror("Failed to read in the buffer\n");
		return 1;
	}
	return 0;
}

float CBMP280::caleTemp(uint32_t adc_T, unsigned short dig_T1, short dig_T2,
		short dig_T3) {
	uint32_t var1, var2;
	float T;
	var1 = (((double) adc_T) / 16384.0 - ((double) dig_T1) / 1024.0)
			* ((double) dig_T2);
	var2 = ((((double) adc_T) / 131072.0 - ((double) dig_T1) / 8192.0)
			* (((double) adc_T) / 131072.0 - ((double) dig_T1) / 8192.0))
			* ((double) dig_T2);
	T = (var1 + var2) / 5120.0;
	return T;
}
