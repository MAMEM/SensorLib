
#include "Recorder.h"
#include <experimental/filesystem>

namespace SensorLib {

	Recorder::Recorder() {

	}

	Recorder::~Recorder() {
		for (size_t i = 0 ; i < currentRecordings.size(); i++)
		{
			delete currentRecordings[i];
		}
		currentRecordings.clear();
	}
	/*
	void Recorder::startRecording(const std::string filename, std::vector<Sensor*> sensors) {
		std::vector<lsl::stream_info> streamsToRecord;
		for (size_t i = 0; i < sensors.size(); i++) {
			if (sensors[i]->getStatus() == STREAMING) {
				lsl::stream_info streaminfo = sensors[i]->getStreamInfo();
				if (&streaminfo != NULL) {
					std::cout << "Recorder: Selected " << sensors[i]->name << "stream for recording" << std::endl;
					streamsToRecord.push_back(streaminfo);
				}
			}
		}
		std::vector<std::string> watchfor;
		std::map<std::string, int> syncOptions;
		std::cout << "Recorder: started recording " << streamsToRecord.size() << " stream(s)" << std::endl;
		//recording *result = new recording(filename, streamsToRecord);
		/*
		if (streamsToRecord.size() > 0) {
			currentRecording = new recording(filename, streamsToRecord, watchfor, syncOptions, true);
		}
		*/
	/*
		for (size_t i = 0; i < streamsToRecord.size() > 0; i++) {
			std::time_t t = std::time(0);
			std::ostringstream ss;
			ss << "../data/" << streamsToRecord[i].name()<< "_" << t << ".xdf";

			std::vector<lsl::stream_info> streamToRecord;
			streamToRecord.push_back(streamsToRecord[i]);
			const std::string filename2 = ss.str();
			currentRecordings.push_back(new recording(filename2, streamToRecord, watchfor, syncOptions, true));
		}
	}
	*/

	recording* Recorder::startRecording(Sensor* sensor)
	{
		std::vector<lsl::stream_info> streamToRecord;
		lsl::stream_info streaminfo = sensor->getStreamInfo();
		if (&streaminfo != NULL) {
			std::cout << "Recorder: startRecording " << sensor->name << std::endl;
			streamToRecord.push_back(streaminfo);
		}
		std::vector<std::string> watchfor;
		std::map<std::string, int> syncOptions;
		
		std::time_t t = std::time(0);
		std::ostringstream ss;
		//ss << ../data/ << streamToRecord[0].name() << "_" << t << ".xdf";
		//ss << "C:\\Users\\MAMEM\\Documents\\" << streamToRecord[0].name() << "_" << t << ".xdf";
		const std::string currentDirectory = std::experimental::filesystem::current_path().generic_string();
		ss << "C://sensordata/" << streamToRecord[0].name() << "_" << t << ".xdf";
		const std::string filename = ss.str();
		
		return new recording(filename, streamToRecord, watchfor, syncOptions, true);
		//currentRecordings.push_back(new recording(filename, streamToRecord, watchfor, syncOptions, true));
	}
	void Recorder::stopRecording() {
		std::cout << "Recorder: stopped recording" << std::endl;
		for (size_t i = 0; i < currentRecordings.size(); i++)
		{
			delete currentRecordings[i];
		}
		currentRecordings.clear();
	}

	void Recorder::recordessr() {

	}

}