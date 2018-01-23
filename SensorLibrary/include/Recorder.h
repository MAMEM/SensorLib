#pragma once
#ifndef RECORDER_H
#define RECORDER_H
#include <atomic>
#include <stdlib.h>
#include <thread>
#include "lsl_cpp.h"
#include <string>
#include "recording.h"
#include "SensorLibrary.h"
#include <boost/algorithm/algorithm.hpp>
#include <vector>
#include <sstream>


namespace SensorLib {
	//enum RecorderStatus {NOT_RECORDING, RECORDING};
	class Recorder {
	public:
		Recorder();
		~Recorder();

		recording* startRecording(Sensor* sensors);
		void stopRecording();

		std::vector<recording*> currentRecordings;


	private:
		void recordessr();

		std::string filename;
		std::atomic<bool> runrecording;
	};
}
#endif