#include "MyGazeGTWSensor.h"
using namespace SensorLib;
__declspec(dllexport) MyGazeGTWSensor::MyGazeGTWSensor(void) {
	type = ET;
	name = "myGazeLSL";
	status = NOT_CONNECTED;
	lslrunning = false;
	numChannels = 2;
	samplingRate = 0;
}

MyGazeGTWSensor::~MyGazeGTWSensor(void) {

}

SensorStatus MyGazeGTWSensor::getStatus() {
	return status;
}

__declspec(dllexport) void MyGazeGTWSensor::connect() {
	status = BUSY;
	lslrunning = true;
	std::cout << "MyGazeGTW: Searching for stream.." << std::endl;
	lsl_thread = new std::thread(&MyGazeGTWSensor::lsl_worker, this);
}

__declspec(dllexport) void  MyGazeGTWSensor::disconnect() {
	lslrunning = false;
}
/*
void MyGazeGTWSensor::statusUpdate(SensorStatus statusUpdate) {
	if (status == statusUpdate)
		return;
	status = statusUpdate;
	if (sLib) {
		sLib->sensorUpdate(this,statusUpdate);
	}
}

void MyGazeGTWSensor::registerStatusUpdateCallback(SensorLibrary * lib) {
	sLib = lib;
}

void MyGazeGTWSensor::unregisterStatusUpdateCallback() {
	if (sLib) {
		sLib = NULL;
	}
}
*/
//LSL inlet will be provided by NIC application, this just checks if it is available
void MyGazeGTWSensor::lsl_worker() {
	SensorStatus tempStatus;
	threadRunning = true;
	while (!shouldShutDown) {
		Sleep(2000);
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", "myGazeLSL", 1, 5);
		tempStatus = NOT_CONNECTED;
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "myGazeLSL")) {
				if (!currentRecording && shouldRecord) {
					std::cout << "MyGazeGTW: Starting recording" << std::endl;
					currentRecording = recorder->startRecording(this);
					statusUpdate(STREAMING);
					tempStatus = RECORDING;
				}
				else if(currentRecording && !shouldRecord){
					statusUpdate(STREAMING);
					tempStatus = STREAMING;
					std::cout << "MyGazeGTW: Stopping recording" << std::endl;
					delete currentRecording;
					currentRecording = NULL;
				}
				else if (currentRecording && shouldRecord) {
					std::cout << "MyGazeGTW: Recording" << std::endl;
					statusUpdate(RECORDING);
					tempStatus = RECORDING;
				}
				else {
					statusUpdate(STREAMING);
					tempStatus = STREAMING;
				}
				break;
			}
		}
		if(tempStatus == NOT_CONNECTED){
			std::cout << "MyGazeGTW: Stream does not exist" << std::endl;
			if (currentRecording) {
				std::cout << "MyGazeGTW: Stopping recording since stream ceased to exist" << std::endl;
				delete currentRecording;
				currentRecording = NULL;
			}
		}
	}
	statusUpdate(NOT_CONNECTED);
	std::cout << "MyGazeGTW: Shutting down.." << std::endl;
	if (currentRecording) {
		std::cout << "MyGazeGTW: Stopping recording before shutting down" << std::endl;
		delete currentRecording;
		currentRecording = NULL;
	}
	threadRunning = false;
}