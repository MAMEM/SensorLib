#include "GazeOutputSensor.h"

using namespace SensorLib;
__declspec(dllexport) GazeOutputSensor::GazeOutputSensor(void) {
	type = MARKERS;
	name = "GazeTheWebOutput";
	status = NOT_CONNECTED;
	lslrunning = false;
	numChannels = 1;
	samplingRate = 0;
}

GazeOutputSensor::~GazeOutputSensor(void) {

}

SensorStatus GazeOutputSensor::getStatus() {
	return status;
}

__declspec(dllexport) void GazeOutputSensor::connect() {
	status = BUSY;
	lslrunning = true;
	std::cout << "GazeTheWebOutput: Searching for stream.." << std::endl;
	lsl_thread = new std::thread(&GazeOutputSensor::lsl_worker, this);
}

__declspec(dllexport) void  GazeOutputSensor::disconnect() {
	lslrunning = false;
}

//LSL inlet will be provided by NIC application, this just checks if it is available
void GazeOutputSensor::lsl_worker() {
	while (!shouldShutDown) {
		Sleep(2000);
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", "GazeTheWebOutput",1,5);
		status = NOT_CONNECTED;
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "GazeTheWebOutput")) {
				std::cout << "GazeTheWebOutput: Connected" << std::endl;
				status = STREAMING;
				if (!currentRecording && shouldRecord) {
					std::cout << "GazeTheWebOutput: Starting recording" << std::endl;
					currentRecording = recorder->startRecording(this);
					status = RECORDING;
				}
				else if (currentRecording && !shouldRecord) {
					status = STREAMING;
					std::cout << "GazeTheWebOutput: Stopping recording" << std::endl;
					delete currentRecording;
					currentRecording = NULL;
				}
				else if (currentRecording && shouldRecord) {
					std::cout << "GazeTheWebOutput: Recording" << std::endl;
					status = RECORDING;
				}
				break;
			}
		}
		if (status == NOT_CONNECTED) {
			std::cout << "MyGazeGTW: Stream does not exist" << std::endl;
			if (currentRecording) {
				std::cout << "GazeOutputSensor: Stopping recording since stream ceased to exist" << std::endl;
				delete currentRecording;
				currentRecording = NULL;
			}
		}
	}
	status = NOT_CONNECTED;
	std::cout << "GazeOutputSensor: Shutting down.." << std::endl;
	if (currentRecording) {
		std::cout << "GazeOutputSensor: Stopping recording before shutting down" << std::endl;
		currentRecording = NULL;
		delete currentRecording;
	}
}