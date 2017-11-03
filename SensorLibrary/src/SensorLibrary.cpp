#include "stdafx.h"  
#include "SensorLibrary.h"  
#include "Recorder.h"
#include "MyGazeSensor.h"
#include "EnobioSensor.h"
#include "ShimmerSensor.h"

namespace SensorLib
{
	SensorLibrary::SensorLibrary(void) {
		sensors.push_back(new MyGazeSensor);
		sensors.push_back(new EnobioSensor);
		sensors.push_back(new ShimmerSensor);
		for (size_t i = 0; i < sensors.size(); i++) {
			sensors[i]->connect();
		}
		recorder = new Recorder();
	}

	SensorLibrary::~SensorLibrary(void) {
		for (size_t i = 0; i < sensors.size(); i++) {
			delete sensors[i];
		}
	}

	void SensorLibrary::printStatus() {
		for (int i = 0; i < sensors.size(); i++) {
			std::cout << "Sensor ID:" << i << "\tName:" << sensors[i]->name << "\tStatus" << sensors[i]->getStatus() << std::endl;
		}
	}

	void SensorLibrary::startRecording(std::string filename)
	{
		recorder->startRecording(filename, sensors);
	}

	void SensorLibrary::stopRecording() {
		recorder->stopRecording();
	}
}
