#include "stdafx.h"
#include "ShimmerSensor.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <sstream>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
using namespace SensorLib;


ShimmerSensor::ShimmerSensor()
{
	type = ET;
	name = "Shimmer";
	device = SHIMMER;
	status = NOT_CONNECTED;
	lslrunning = false;
	numChannels = 2;
	samplingRate = 256;
}


ShimmerSensor::~ShimmerSensor()
{
}


SensorStatus ShimmerSensor::getStatus() {
	return status;
}

void ShimmerSensor::connect() {
	status = BUSY;
	lslrunning = true;
	lsl_thread = new std::thread(&ShimmerSensor::lsl_worker, this);

}

void ShimmerSensor::disconnect() {
	lslrunning = false;
}

void ShimmerSensor::lsl_worker() {
	//SystemInfoStruct systemInfoData;
	int ret_connect = 0;
	
	if (NULL != _dataCollector)
	{
		delete _dataCollector;
		_dataCollector = NULL;
	}

	_dataCollector = new DeviceDataCollector();
	bool found = false;
	status = BUSY;
	for (int i = 1; i < 10; i++) {
		std::ostringstream oss;
		oss << "COM" << i;
		unsigned int error = _dataCollector->Open(oss.str());
		//std::cout << "Trying COM" << i << std::endl;
		//std::cout << error << std::endl;
		if (error != IO_NO_ERROR)
		{

			//MessageBox::Show(String::Format("Open communication failed! Hardware Error ( code: 0x{0:X} )", error), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			continue;
		}
		error = _dataCollector->SetAcquisitionMode(ACQUISITION);

		if (error != IO_NO_ERROR)
		{
			//MessageBox::Show(String::Format("Set acquisition mode failed! Hardware Error ( code: 0x{0:X} )", error), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			//_btnLink->Checked = false;
			continue;
		}
		std::cout << "Connected to Shimmer" << std::endl;
		found = true;
		break;

	}
	if (!found) {
		status = ERR;
		return;
	}
	status = STREAMING;

	while (lslrunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}


	status = CONNECTED;

	if (NULL == _dataCollector)
		return;

	_dataCollector->SetAcquisitionMode(IDLE);

	_dataCollector->Close();

	status = NOT_CONNECTED;
}


