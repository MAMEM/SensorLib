#include "MyGazeSensor.h"

using namespace SensorLib;
lsl::stream_outlet* MyGazeSensor::upOutlet;
 MyGazeSensor::MyGazeSensor(void) {
	type = ET;
	name = "MyGaze";
	device = MYGAZE;
	status = NOT_CONNECTED;
	lslrunning = false;
	numChannels = 14;
	samplingRate = 30;
}

MyGazeSensor::~MyGazeSensor(void) {

}

SensorStatus MyGazeSensor::getStatus() {
	return status;
}

void MyGazeSensor::connect() {
	status = BUSY;
	std::cout << "connect" << std::endl;
	lslrunning = true;
	lsl_thread = new std::thread(&MyGazeSensor::lsl_worker, this);
	std::cout << "MyGaze: dispatched thread" << std::endl;
}

void MyGazeSensor::disconnect() {
	lslrunning = false;
}

void MyGazeSensor::lsl_worker() {
	SystemInfoStruct systemInfoData;
	int ret_connect = 0;
	while (ret_connect != RET_SUCCESS)
	{
		// Connect to myGaze
		ret_connect = iV_Connect();
	}
	status = CONNECTED;
	iV_GetSystemInfo(&systemInfoData);
	samplingRate = systemInfoData.samplerate;
	lsl::stream_info streamInfo(name, "EyeTracker", numChannels, samplingRate, lsl::cf_float32, "mysource333645");
	//set sample callback
	streamInfo.desc().append_child_value("manufacturer", "Visual Interaction GmbH");

	lsl::xml_element time = streamInfo.desc().append_child("event_synchronization");
	//unix timestamp in seconds;
	std::ostringstream unixtimestamp;
	unixtimestamp.precision(15);
	unixtimestamp << std::time(0);
	//unix timestamp in milliseconds
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	std::ostringstream unixtimestampms;
	unixtimestampms.precision(15);
	unixtimestampms << ms.count();
	//lsl local clock
	std::ostringstream lslclock;
	lslclock.precision(15);
	lslclock << lsl_local_clock();
	//offset in milliseconds
	std::ostringstream timeoffset;
	timeoffset.precision(15);
	timeoffset << (ms.count() - lsl_local_clock());
	time.append_child_value("unixtimestamp", unixtimestamp.str());
	time.append_child_value("unixtimestmapms", unixtimestampms.str());
	time.append_child_value("lslclock", lslclock.str());
	time.append_child_value("streamoffset", timeoffset.str());


	lsl::xml_element channels = streamInfo.desc().append_child("channels");
	channels.append_child("channel")
		.append_child_value("label", "gazeX")
		.append_child_value("unit", "screenPixels");
	channels.append_child("channel")
		.append_child_value("label", "gazeY")
		.append_child_value("unit", "screenPixels");
	channels.append_child("channel")
		.append_child_value("label", "leftgazeX")
		.append_child_value("unit", "screenPixels");
	channels.append_child("channel")
		.append_child_value("label", "leftgazeY")
		.append_child_value("unit", "screenPixels");
	channels.append_child("channel")
		.append_child_value("label", "rightgazeX")
		.append_child_value("unit", "screenPixels");
	channels.append_child("channel")
		.append_child_value("label", "rightgazeY")
		.append_child_value("unit", "screenPixels");
	channels.append_child("channel")
		.append_child_value("label", "leftPosX")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "leftPosY")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "leftPosZ")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "rightPosX")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "rightPosY")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "rightPosZ")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "leftDiameter")
		.append_child_value("unit", "mm");
	channels.append_child("channel")
		.append_child_value("label", "rightDiameter")
		.append_child_value("unit", "mm");
	//lsl::stream_outlet outlet(streamInfo);
	//upOutlet = lsl::stream_outlet(streamInfo);
	MyGazeSensor::upOutlet = new lsl::stream_outlet(streamInfo);
	//MyGazeSensor::myGazeOutlet = std::unique_ptr<lsl::stream_outlet>(new lsl::stream_outlet(streamInfo));
	iV_SetSampleCallback(MyGazeSensor::SampleCallbackFunction);
	status = STREAMING;
	while (lslrunning) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	iV_SetSampleCallback(NULL);
	status = CONNECTED;
	iV_Disconnect();
	status = NOT_CONNECTED;
}


int __stdcall MyGazeSensor::SampleCallbackFunction(SampleStruct sampleData)
{
	std::vector<double> values;
	values.push_back(std::max(sampleData.leftEye.gazeX, sampleData.rightEye.gazeX));
	values.push_back(std::max(sampleData.leftEye.gazeY, sampleData.rightEye.gazeY));
	values.push_back(sampleData.leftEye.gazeX);
	values.push_back(sampleData.leftEye.gazeY);
	values.push_back(sampleData.rightEye.gazeX);
	values.push_back(sampleData.rightEye.gazeY);
	values.push_back(sampleData.leftEye.eyePositionX);
	values.push_back(sampleData.leftEye.eyePositionY);
	values.push_back(sampleData.leftEye.eyePositionZ);
	values.push_back(sampleData.rightEye.eyePositionX);
	values.push_back(sampleData.rightEye.eyePositionY);
	values.push_back(sampleData.rightEye.eyePositionZ);
	values.push_back(sampleData.leftEye.diam);
	values.push_back(sampleData.rightEye.diam);
	// Push to outlet stream
	if (MyGazeSensor::upOutlet != nullptr)
	{
		//std::cout << "Sample received" << std::endl;
		MyGazeSensor::upOutlet->push_sample(values);
	}
	// Why one, why not zero?
	return 1;
}
