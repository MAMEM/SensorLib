#include "EmotivSensor.h"
//#define EPOC__
using namespace SensorLib;
EmotivSensor::EmotivSensor(void) {
	type = EEG;
#ifdef EPOC__
	name = "EPOC";
	device = EMOTIV;
#else
	name = "Insight";
	device = EMOTIV;
#endif
	status = NOT_CONNECTED;
	lslrunning = false;
#ifdef EPOC__
	numChannels = 14;
#else
	numChannels = 10;
#endif
	samplingRate = 128;
}

EmotivSensor::~EmotivSensor(void) {

}

SensorStatus EmotivSensor::getStatus() {
	return status;
}

void EmotivSensor::connect() {
	status = BUSY;
	lslrunning = true;
	lsl_thread = new std::thread(&EmotivSensor::lsl_worker,this);
}

void  EmotivSensor::disconnect() {
	lslrunning = false;
}

void EmotivSensor::lsl_worker() {
#ifdef EPOC__
	IEE_DataChannel_t EpocChannelList[] = {

		IED_COUNTER, IED_INTERPOLATED,
		IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7,
		IED_P7, IED_O1, IED_O2, IED_P8, IED_T8,
		IED_FC6, IED_F4, IED_F8, IED_AF4, IED_RAW_CQ,
		IED_GYROX, IED_GYROY, IED_MARKER, IED_TIMESTAMP
	};
#else
	IEE_DataChannel_t InsightChannelList[] = {

		IED_COUNTER,
		IED_INTERPOLATED,
		IED_RAW_CQ,
		IED_AF3,
		IED_T7,
		IED_Pz,
		IED_T8,
		IED_AF4,
		IED_TIMESTAMP,
		IED_MARKER,
		IED_SYNC_SIGNAL
	};
#endif
	const char header[] = "	IED_COUNTER, IED_INTERPOLATED, IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7, "
		"IED_P7, IED_O1, IED_O2, IED_P8, IED_T8, IED_FC6, IED_F4, IED_F8, IED_AF4, IED_RAW_CQ, "
		"IED_GYROX, IED_GYROY, IED_MARKER, IED_TIMESTAMP ";
	const char* channels[] = { "AF3", "F7", "F3", "FC5", "T7", "P7", "O1", "O2", "P8", "T8", "FC6", "F4", "F8", "AF4" };
#ifdef EPOC__
	//EPOC
	const int channelids[14] = { 2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const int channellength = 14;
#else
	//INSIGHT
	const int channelids[11] = {0,1,2,3,4,5,6,7,8,9};
	const int channellength = 10;
#endif
	int state = 0;
	EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();
	bool readytocollect = false;
	unsigned int userID = 0;
	float secs = 1;
	if (IEE_EngineConnect() != EDK_OK) {
		std::cerr << "Emotiv Driver start up failed." << std::endl;
		status = ERR;
		return;
	}
	DataHandle hData = IEE_DataCreate();
	try {
		IEE_DataSetBufferSizeInSec(secs);
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		status = ERR;
		return;
	}
#ifdef EPOC__
	lsl::stream_info outletInfo("EPOC", "Markers", numChannels, samplingRate, lsl::cf_float32, "unid4457");
#else
	lsl::stream_info outletInfo("Insight", "Markers", numChannels, samplingRate, lsl::cf_float32, "unid4457");
#endif
	outletInfo.desc().append_child_value("manufacturer", "Emotiv");
	lsl::xml_element chns = outletInfo.desc().append_child("channels");
	for (int i = 0; i < channellength; i++) {
		chns.append_child("channel")
			.append_child_value("label", channels[i])
			.append_child_value("unit", "microvolts")
			.append_child_value("type", "EEG");
	}
	lsl::stream_outlet outlet(outletInfo);
	//while(false){
	while (lslrunning) {
		state = IEE_EngineGetNextEvent(eEvent);
		if (state == EDK_OK) {
			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);
			if (eventType == IEE_UserAdded) {
				std::cout << "User added" << std::endl;
				status = CONNECTED;
				IEE_DataAcquisitionEnable(userID, true);
				readytocollect = true;
			}
		}
		if (readytocollect) {
			IEE_DataUpdateHandle(0, hData);
			unsigned int nSamplesTaken = 0;
			IEE_DataGetNumberOfSample(hData, &nSamplesTaken);
			//std::cout << "Updated " << nSamplesTaken << std::endl;
			
			if (nSamplesTaken != 0) {
				double* data = new double[nSamplesTaken];
				for (int sampleIdx = 0; sampleIdx < (int)nSamplesTaken; ++sampleIdx) {
					std::vector<double> samplevec;
					/*for (int i = 0; i < sizeof(EpocChannelList) / sizeof(IEE_DataChannel_t); i++) {
					IEE_DataGet(hData, EpocChannelList[i], data, nSamplesTaken);
					samplevec.push_back(data[sampleIdx]);
					}*/
					for (int i = 0; i < channellength; i++) {
						//						std::cout << "id = " << EpocChannelList[channzids[i]] << std::endl;
#ifdef EPOC__
						IEE_DataGet(hData, EpocChannelList[channelids[i]], data, nSamplesTaken);
#else
						IEE_DataGet(hData, InsightChannelList[channelids[i]], data, nSamplesTaken);
#endif
						samplevec.push_back(data[sampleIdx]);
					}
					//						system("pause");
					//						std::cout << "test" << std::endl;
					if (nSamplesTaken > 0) {
						status = STREAMING;
					}
					outlet.push_sample(samplevec);
				}
				delete[] data;
			}
			Sleep(1);
		}
		if (!readytocollect) 
			Sleep(5000);
	}
	status = BUSY;
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
	status = NOT_CONNECTED;
}