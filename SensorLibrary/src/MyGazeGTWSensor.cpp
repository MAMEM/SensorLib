#include "MyGazeGTWSensor.h"

using namespace SensorLib;
__declspec(dllexport) MyGazeGTWSensor::MyGazeGTWSensor(void) {
	type = MARKERS;
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
		std::vector<lsl::stream_info> results = lsl::resolve_stream("name", "myGazeLSL", 1, 5);
		status = NOT_CONNECTED;
		for (size_t i = 0; i < results.size(); i++) {
			if (!strcmp(results[i].name().c_str(), "myGazeLSL")) {
				std::cout << "MyGazeGTW: Streaming" << std::endl;
				status = STREAMING;
			}
			else {
				status = NOT_CONNECTED;
			}
		}
		Sleep(10000);
		if (status == NOT_CONNECTED) {
			std::cout << "MyGazeGTW: Not connected" << std::endl;
		}
		//lsl::stream_inlet inlet(results[0]);
	}
}