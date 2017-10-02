#include "EnobioSensor.h"
//#define EPOC__
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
	std::cout << "connect" << std::endl;
	lslrunning = true;
	lsl_thread = new std::thread(&EnobioSensor::lsl_worker, this);
	std::cout << "dispatched thread" << std::endl;
}

__declspec(dllexport) void  EnobioSensor::disconnect() {
	lslrunning = false;
}

//LSL inlet will be provided by NIC application, this just checks if it is available
void EnobioSensor::lsl_worker() {
	while (lslrunning) {
		std::vector<lsl::stream_info> results = lsl::resolve_stream("type", "EEG");
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "enobio")) {
				status = STREAMING;
				//std::cout << "status:Streaming" << std::endl;
			}
			else {
				status = NOT_CONNECTED;
				//std::cout << "status:NotConnected" << std::endl;
			}
		}
		Sleep(10000);
		//lsl::stream_inlet inlet(results[0]);
	}
}