#pragma once
#ifndef MYGAZE_SENSOR_H
#define MYGAZE_SENSOR_H
#include "Sensor.h"

#include "sensor/mygaze/myGazeAPI.h"
#include "lsl_cpp.h"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <memory>

namespace SensorLib {
	//DEBUG this
	
	class MyGazeSensor : public Sensor {
	public:
		__declspec(dllexport) MyGazeSensor();
		~MyGazeSensor();
		__declspec(dllexport) virtual SensorStatus getStatus();
		__declspec(dllexport) virtual void connect();
		__declspec(dllexport) virtual void disconnect();
		//std::unique_ptr<lsl::stream_outlet> MyGazeSensor::myGazeOutlet;
		static int __stdcall SampleCallbackFunction(SampleStruct sampleData);
	protected:
		virtual void lsl_worker();
		static lsl::stream_outlet* upOutlet;
		
	};
}
#endif