//============================================================================
// Name        : 335x_Plane.cpp
// Author      : JohnRen
// Version     :
// Copyright   : JohnRen
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <unistd.h>
#include <stdio.h>

#include "./drivers/common/Barometer/CBarometer.h"
#include "./drivers/BMP280/CBMP280.h"

CBarometer *g_pBaro = new CBMP280;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	g_pBaro->Init();

	float fTemperature;

	while (1) {
		g_pBaro->GetTemperature(fTemperature);
		printf("Temperature is : %f \n", fTemperature);

		usleep(100);
	}

	return 0;
}
