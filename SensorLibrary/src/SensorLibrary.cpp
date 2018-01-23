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
		recorder = std::shared_ptr<Recorder>(new Recorder());
		for (size_t i = 0; i < sensors.size(); i++) {
			sensors[i]->connect();
			sensors[i]->recorder = recorder;
			sensors[i]->shouldRecord = true;
		}
		
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

	void SensorLibrary::startRecording()
	{
		/*
		std::wstring outputFolder = L"../data";
		if (CreateDirectory(outputFolder.c_str(), NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			std::time_t t = std::time(0);
			std::ostringstream oss;
			oss << "../data/" << t << ".xdf";
			//actual filename generated afterwards
//			recorder->startRecording(oss.str(), sensors);
		}
		*/
	}

	void SensorLibrary::stopRecording() {
		for (int i = 0; i < sensors.size(); i++) {
			sensors[i]->shouldRecord = false;
		}
//		recorder->stopRecording();
	}

	void SensorLibrary::shutdownSensors() {
		for (int i = 0; i < sensors.size(); i++) {
			std::cout << "SensorLibrary: Shutting down the sensors" << std::endl;
			sensors[i]->shouldShutDown = true;
		}
	}
}
