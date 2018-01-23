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

//LSL inlet will be provided by NIC application, this just checks if it is available
void MyGazeGTWSensor::lsl_worker() {
	while (lslrunning) {
		Sleep(2000);
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", "myGazeLSL", 1, 5);
		status = NOT_CONNECTED;
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "myGazeLSL")) {
				std::cout << "MyGazeGTW: Connected" << std::endl;
				status = STREAMING;
				if (!currentRecording && shouldRecord) {
					std::cout << "MyGazeGTW: Starting recording" << std::endl;
					currentRecording = recorder->startRecording(this);
					status = RECORDING;
				}
				else if(currentRecording && !shouldRecord){
					status = STREAMING;
					std::cout << "MyGazeGTW: Stopping recording" << std::endl;
					delete currentRecording;
					currentRecording = NULL;
				}
				else if (currentRecording && shouldRecord) {
					std::cout << "MyGazeGTW: Recording" << std::endl;
					status = RECORDING;
				}
				break;
			}
		}
		if(status == NOT_CONNECTED){
			std::cout << "MyGazeGTW: Stream does not exist" << std::endl;
			if (currentRecording) {
				std::cout << "MyGazeGTW: Stopping recording since stream ceased to exist" << std::endl;
				delete currentRecording;
				currentRecording = NULL;
			}
		}
	}
	status = NOT_CONNECTED;
	std::cout << "MyGazeGTW: Shutting down.." << std::endl;
	if (currentRecording) {
		std::cout << "MyGazeGTW: Stopping recording before shutting down" << std::endl;
		delete currentRecording;
		currentRecording = NULL;
	}
}