#include "stdafx.h"  
#include "SensorLibrary.h"  
#include "Recorder.h"
//#include "MyGazeSensor.h"
#include "MyGazeGTWSensor.h"
#include "EnobioSensor.h"
#include "ShimmerSensor.h"
#include "GazeOutputSensor.h"
namespace SensorLib
{
	SensorLibrary::SensorLibrary(void) {

		sensors.push_back(new MyGazeGTWSensor);
		sensors.push_back(new EnobioSensor);
		sensors.push_back(new ShimmerSensor);
		sensors.push_back(new GazeOutputSensor);
		for (size_t i = 0; i < sensors.size(); i++) {
			sensors[i]->connect();
		}
		recorder = new Recorder();
	}

	SensorLibrary::~SensorLibrary(void) {
		for (size_t i = 0; i < sensors.size(); i++) {
			delete sensors[i];
		}
		delete recorder;
	}

	void SensorLibrary::printStatus() {
		for (int i = 0; i < sensors.size(); i++) {
			std::cout << "Sensor ID:" << i << "\tName:" << sensors[i]->name << "\tStatus" << sensors[i]->getStatus() << std::endl;
		}
	}

	void SensorLibrary::startRecording()
	{
		std::wstring outputFolder = L"../data";
		if (CreateDirectory(outputFolder.c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			std::time_t t = std::time(0);
			std::ostringstream oss;
			oss << "../data/" << t << ".xdf";
			recorder->startRecording(oss.str(), sensors);
		}
	}

	void SensorLibrary::stopRecording() {
		recorder->stopRecording();
	}
}
