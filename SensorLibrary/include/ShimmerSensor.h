#pragma once
#ifndef SHIMMER_SENSOR_H
#define SHIMMER_SENSOR_H
#include "Sensor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
#include <string>
#include "SDK_Shimmer.h"
namespace SensorLib {
	class ShimmerSensor : public Sensor {
	public:
		__declspec(dllexport) ShimmerSensor();
		~ShimmerSensor();
		__declspec(dllexport) virtual SensorStatus getStatus();
		__declspec(dllexport) virtual void connect();
		__declspec(dllexport) virtual void disconnect();
	protected:
		virtual void lsl_worker();
	};
}
#endif