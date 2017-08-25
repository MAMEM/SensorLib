#pragma once
#ifndef MYGAZE_SENSOR_H
#define MYGAZE_SENSOR_H
#include "Sensor.h"

#include <string>
#include <stdlib.h>
#include <iostream>
namespace SensorLib {
	class MyGazeSensor : public Sensor {
	public:
		__declspec(dllexport) MyGazeSensor();
		~MyGazeSensor();
		__declspec(dllexport) virtual SensorStatus getStatus();
		__declspec(dllexport) virtual void connect();
		__declspec(dllexport) virtual void disconnect();
		__declspec(dllexport) virtual void record(std::string filename);
		__declspec(dllexport) virtual void stopRecording(std::string filename);

	protected:
		virtual void lsl_worker();
	};
}
#endif