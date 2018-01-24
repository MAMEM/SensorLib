#include "EnobioSensor.h"

using namespace SensorLib;
__declspec(dllexport) EnobioSensor::EnobioSensor(void) {
	type = EEG;
	name = "enobio";
	status = NOT_CONNECTED;
	lslrunning = false;
	numChannels = 8;
	samplingRate = 500;
}

EnobioSensor::~EnobioSensor(void) {

}

SensorStatus EnobioSensor::getStatus() {
	return status;
}

__declspec(dllexport) void EnobioSensor::connect() {
	status = BUSY;
	lslrunning = true;
	lsl_thread = new std::thread(&EnobioSensor::lsl_worker, this);
	std::cout << "Enobio: Searching for stream.." << std::endl;
}

__declspec(dllexport) void  EnobioSensor::disconnect() {
	lslrunning = false;
}

//LSL inlet will be provided by NIC application, this just checks if it is available
void EnobioSensor::lsl_worker() {
	SensorStatus tempStatus;
	while (lslrunning) {
		Sleep(2000);
		std::vector<lsl::stream_info> results = lsl::resolve_stream("type", "EEG",1,5);
		tempStatus = NOT_CONNECTED;
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "enobio")) {
				if (!currentRecording && shouldRecord) {
					std::cout << "Enobio: Starting recording" << std::endl;
					currentRecording = recorder->startRecording(this);
					statusUpdate(STREAMING);
					tempStatus = RECORDING;
				}
				else if (currentRecording && !shouldRecord) {
					statusUpdate(STREAMING);
					tempStatus = STREAMING;
					std::cout << "Enobio: Stopping recording" << std::endl;
					delete currentRecording;
					currentRecording = NULL;
				}
				else if (currentRecording && shouldRecord) {
					std::cout << "Enobio: Recording" << std::endl;
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
		if (status == NOT_CONNECTED) {
			std::cout << "Enobio: Stream does not exist" << std::endl;
			if (currentRecording) {
				std::cout << "Enobio: Stopping recording since stream ceased to exist" << std::endl;
				delete currentRecording;
				currentRecording = NULL;
			}
		}
	}
	status = NOT_CONNECTED;
	std::cout << "Enobio: Shutting down.." << std::endl;
	if (currentRecording) {
		std::cout << "Enobio: Stopping recording before shutting down" << std::endl;
		delete currentRecording;
		currentRecording = NULL;
	}
}