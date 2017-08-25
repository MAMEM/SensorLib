
#include "Recorder.h"

namespace SensorLib {

	Recorder::Recorder() {

	}

	Recorder::~Recorder() {

	}

	void Recorder::startRecording(const std::string filename, std::vector<Sensor*> sensors) {
		std::cout << "0" << std::endl;
		std::vector<lsl::stream_info> streamsToRecord;
		std::cout << "1" << std::endl;
		for (size_t i = 0; i < sensors.size(); i++) {
			if (sensors[i]->getStatus() == STREAMING) {
				std::cout << "2" << std::endl;
				lsl::stream_info streaminfo = sensors[i]->getStreamInfo();
				std::cout << "3" << std::endl;
				if (&streaminfo != NULL) {
					std::cout << "4" << std::endl;
					streamsToRecord.push_back(streaminfo);
					std::cout << "4.5" << std::endl;
				}
			}
		}
		std::cout << "5" << std::endl;
		std::vector<std::string> watchfor;
		std::map<std::string, int> syncOptions;

		//recording *result = new recording(filename, streamsToRecord);
		std::cout << "6" << std::endl;
		recording *result = new recording(filename, streamsToRecord, watchfor, syncOptions, true);
		std::cout << "7" << std::endl;
	}

	void Recorder::stopRecording() {

	}

	void Recorder::recordessr() {

	}

}