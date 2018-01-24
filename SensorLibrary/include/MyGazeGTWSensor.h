#pragma once
#ifndef GAZE_GTW_SENSOR_H
#define GAZE_GTW_SENSOR_H
#include "Sensor.h"

#include "lsl_cpp.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <Windows.h>
#include <string>
#include "Recorder.h"

namespace SensorLib {
	//DEBUG this

	class MyGazeGTWSensor : public Sensor {
	public:
		__declspec(dllexport) MyGazeGTWSensor();
		~MyGazeGTWSensor();
		__declspec(dllexport) virtual SensorStatus getStatus();
		__declspec(dllexport) virtual void connect();
		__declspec(dllexport) virtual void disconnect();
		/*
		void statusUpdate(SensorStatus statusUpdate);
		void registerStatusUpdateCallback(SensorLibrary * lib);
		void unregisterStatusUpdateCallback();
		*/
		//std::unique_ptr<lsl::stream_outlet> MyGazeSensor::myGazeOutlet;
	protected:
		virtual void lsl_worker();
		static lsl::stream_outlet* upOutlet;

	};
}
#endif