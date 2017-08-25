#include "MyGazeSensor.h"

using namespace SensorLib;
__declspec(dllexport) MyGazeSensor::MyGazeSensor(void) {
	type = ET;
	name = "MyGaze";
	device = MYGAZE;
	status = NOT_CONNECTED;
	lslrunning = false;
	numChannels = 4;
	samplingRate = 30;
}

MyGazeSensor::~MyGazeSensor(void) {

}

SensorStatus MyGazeSensor::getStatus() {
	return status;
}

__declspec(dllexport) void MyGazeSensor::connect() {
	status = BUSY;
	std::cout << "connect" << std::endl;
	lslrunning = true;
	lsl_thread = new std::thread(&MyGazeSensor::lsl_worker, this);
	std::cout << "MyGaze: dispatched thread" << std::endl;
}

__declspec(dllexport) void MyGazeSensor::disconnect() {
	lslrunning = false;
}

void MyGazeSensor::record(std::string filename) {

}

void MyGazeSensor::stopRecording(std::string filename) {

}

void MyGazeSensor::lsl_worker() {

}