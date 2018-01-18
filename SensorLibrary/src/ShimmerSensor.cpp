#include "stdafx.h"
#include "ShimmerSensor.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <sstream>

#include "enumser.h"

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
	std::cout << "Shimmer: Searching for stream" << std::endl;

}

void ShimmerSensor::disconnect() {
	lslrunning = false;
}

std::vector<std::wstring> ShimmerSensor::getSerialPorts() {
	std::vector<UINT> portList;
	std::vector<std::wstring> namesList;
	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		std::cout << "Shimmer: error getting serial ports" << std::endl;
		return namesList;
	}
	hr = CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);
	if (FAILED(hr))
	{
		CoUninitialize();
		std::cout << "Shimmer: error getting serial ports 2" << std::endl;
		std::cout << GetLastError() << std::endl;
		return namesList;
	}
	CEnumerateSerial::CPortsArray ports;
	CEnumerateSerial::CNamesArray names;
	if (CEnumerateSerial::UsingRegistry(names))
	{
		for (size_t i = 0; i < names.size(); i++) {
			namesList.push_back(names[i].c_str());
		}
	}
	std::cout << "Shimmer: namesList size = " << namesList.size() << std::endl;
	CoUninitialize();
	return namesList;
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
	std::vector<std::wstring> ports = getSerialPorts();
searchShimmer:
	if (ports.size() == 0) {
		ports = getSerialPorts();
	}
	
	std::cout << "Shimmer: serial ports size =  " << ports.size() << std::endl;
	for (int i = 0; i < ports.size(); i++) {
		//std::ostringstream oss;
		//oss << "COM" << ports[i];
		unsigned int error = _dataCollector->Open(ports[i]);
		std::cout << "Trying COM" << i << std::endl;
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

		found = true;
		break;

	}
	if (!found) {
		status = ERR;
		std::cout << "Shimmer: Not Connected" << std::endl;
		Sleep(5000);
		goto searchShimmer;
		return;
	}
	std::cout << "Shimmer: Connected" << std::endl;
	status = STREAMING;

	while (lslrunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}


	status = CONNECTED;

	if (NULL == _dataCollector)
		return;

	_dataCollector->SetAcquisitionMode(IDLE);

	_dataCollector->Close();
	std::cout << "Shimmer: Disconnected" << std::endl;
	status = NOT_CONNECTED;
}


