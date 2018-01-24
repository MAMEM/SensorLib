#include "stdafx.h"  
#include "SensorLibrary.h"  
#include "Recorder.h"
//#include "MyGazeSensor.h"
#include "MyGazeGTWSensor.h"
#include "EnobioSensor.h"
//#include "ShimmerSensor.h"
#include "GazeOutputSensor.h"


namespace SensorLib
{
	SensorLibrary::SensorLibrary(void) {

		sensors.push_back(new MyGazeGTWSensor);
		sensors.push_back(new EnobioSensor);
		//sensors.push_back(new ShimmerSensor);
		sensors.push_back(new GazeOutputSensor);
		recorder = std::shared_ptr<Recorder>(new Recorder());
		for (size_t i = 0; i < sensors.size(); i++) {
			sensors[i]->connect();
			sensors[i]->recorder = recorder;
			sensors[i]->shouldRecord = true;
			sensors[i]->registerStatusUpdateCallback(this);
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
		for (int i = 0; i < sensors.size(); i++) {
			sensors[i]->shouldRecord = true;
		}
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
		int i = 0;
		while (true) {
			bool isRecording = false;
			for (int i = 0; i < sensors.size(); i++) {
				if (sensors[i]->getStatus() == RECORDING) {
					isRecording = true;
				}
			}
			if (isRecording == false) {
				break;
			}
			Sleep(500);
			i++;
			if (i > 20) {
				std::cout << "SensorLibrary: Could not stop recording shutting down anyway" << std::endl;
				break;
			}
		}
//		recorder->stopRecording();
	}

	void SensorLibrary::shutdownSensors() {
		for (int i = 0; i < sensors.size(); i++) {
			std::cout << "SensorLibrary: Shutting down the sensors" << std::endl;
			sensors[i]->shouldShutDown = true;
		}
	}
	void SensorLibrary::sensorUpdate(Sensor *sensor, SensorStatus status) {
		std::cout << sensor->name << ": status upd " << status << std::endl;
	}
}
