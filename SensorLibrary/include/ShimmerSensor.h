#pragma once
#ifndef SHIMMERSENSOR_H
#define SHIMMERSENSOR_H
#include "Sensor.h"
#include <sensor/shimmer/DeviceDataCollector.h>

#include "lsl_cpp.h"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <memory>

namespace SensorLib 
{
	//DEBUG this

	class ShimmerSensor : public Sensor {

		std::string _com;
		DeviceDataCollector * _dataCollector;

	public:
		ShimmerSensor();
		~ShimmerSensor();
		virtual SensorStatus getStatus();
		virtual void connect();
		virtual void disconnect();
		
		
	protected:
		virtual void lsl_worker();
		static lsl::stream_outlet* upOutlet;
		void QueryKey(HKEY hKey);

	};
}
#endif