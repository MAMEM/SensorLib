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
	std::cout << "connect" << std::endl;
	lslrunning = true;
	lsl_thread = new std::thread(&GazeOutputSensor::lsl_worker, this);
	std::cout << "dispatched thread" << std::endl;
}

__declspec(dllexport) void  GazeOutputSensor::disconnect() {
	lslrunning = false;
}

//LSL inlet will be provided by NIC application, this just checks if it is available
void GazeOutputSensor::lsl_worker() {
	while (lslrunning) {
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", "GazeTheWebOutput");
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "GazeTheWebOutput")) {
				std::cout << "GazeTheWebOutput: Streaming" << std::endl;
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