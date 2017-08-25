#pragma once
#ifndef INSIGHT_SENSOR_H
#define INSIGHT_SENSOR_H
#include "Sensor.h"

#include "sensor\emotiv\IEegData.h"
#include "sensor\emotiv\Iedk.h"
#include "sensor\emotiv\IedkErrorCode.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
namespace SensorLib {
	class EmotivSensor : public Sensor {
	public:	
		__declspec(dllexport) EmotivSensor();
		~EmotivSensor();
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