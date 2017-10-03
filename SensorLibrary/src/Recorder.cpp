
#include "Recorder.h"

namespace SensorLib {

	Recorder::Recorder() {

	}

	Recorder::~Recorder() {

	}

	void Recorder::startRecording(const std::string filename, std::vector<Sensor*> sensors) {
		std::vector<lsl::stream_info> streamsToRecord;
		for (size_t i = 0; i < sensors.size(); i++) {
			if (sensors[i]->getStatus() == STREAMING) {
				lsl::stream_info streaminfo = sensors[i]->getStreamInfo();
				if (&streaminfo != NULL) {
					std::cout << "recording stream: " << sensors[i]->name;
					streamsToRecord.push_back(streaminfo);
				}
			}
		}
		std::vector<std::string> watchfor;
		std::map<std::string, int> syncOptions;
		std::cout << "recording " << streamsToRecord.size() << " streams" << std::endl;
		//recording *result = new recording(filename, streamsToRecord);
		recording *result = new recording(filename, streamsToRecord, watchfor, syncOptions, true);
	}

	void Recorder::stopRecording() {

	}

	void Recorder::recordessr() {

	}

}