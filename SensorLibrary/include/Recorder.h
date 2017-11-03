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


namespace SensorLib {
	enum RecorderStatus {NOT_RECORDING, RECORDING};
	class Recorder {
	public:
		Recorder();
		~Recorder();

		void startRecording(std::string filename, std::vector<Sensor*> sensors);
		void stopRecording();

		recording* currentRecording;


	private:
		void recordessr();

		std::string filename;
		std::atomic<bool> runrecording;
		std::atomic<RecorderStatus> status;
	};
}
#endif