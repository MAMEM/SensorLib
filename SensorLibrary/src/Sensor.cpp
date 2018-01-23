#pragma once
#include "Sensor.h"
#include <stdlib.h>
using namespace SensorLib;
Sensor::Sensor(void) {
	shouldRecord = false;
	shouldShutDown = false;
}
Sensor::~Sensor(void) {
	if (currentRecording) {
		delete currentRecording;
	}
}

lsl::stream_info Sensor::getStreamInfo() {
	std::vector<lsl::stream_info> streaminfos;
	if (!strcmp(name.c_str(), "enobio")) {
		streaminfos = lsl::resolve_stream("type", "EEG");
		for (size_t i = 0; i < streaminfos.size(); i++) {
			if (!strcmp(streaminfos[i].name().c_str(), "enobio")) {
				return streaminfos[i];
			}
		}
	}
	streaminfos = lsl::resolve_stream("name", name); // search for stream with certain name
	if (streaminfos.size() != 1) {
		return NULL;
	}
	else {
		return streaminfos[0];
	}
}