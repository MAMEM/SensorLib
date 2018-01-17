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
	while (lslrunning) {
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", "GazeTheWebOutput",1,5);
		status = NOT_CONNECTED;
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "GazeTheWebOutput")) {
				std::cout << "GazeTheWebOutput: Streaming" << std::endl;
				status = STREAMING;
			}
			else {
				status = NOT_CONNECTED;
			}
		}
		Sleep(10000);
		if (status == NOT_CONNECTED) {
		std::cout << "GazeTheWebOutput: Not connected" << std::endl;
		}
		//lsl::stream_inlet inlet(results[0]);
	}
}