#pragma once
#ifndef ENOBIO_SENSOR_H
#define ENOBIO_SENSOR_H
#include "Sensor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
#include <string>
namespace SensorLib {
	class EnobioSensor : public Sensor {
	public:
		__declspec(dllexport) EnobioSensor();
		~EnobioSensor();
		__declspec(dllexport) virtual SensorStatus getStatus();
		__declspec(dllexport) virtual void connect();
		__declspec(dllexport) virtual void disconnect();
	protected:
		virtual void lsl_worker();
	};
}
#endif