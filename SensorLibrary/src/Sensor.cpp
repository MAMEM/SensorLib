#pragma once
#include "Sensor.h"
using namespace SensorLib;
Sensor::Sensor(void) {
}
Sensor::~Sensor(void) {
}

lsl::stream_info Sensor::getStreamInfo() {
	std::vector<lsl::stream_info> streaminfos;
	streaminfos = lsl::resolve_stream("name", name); // search for stream with certain name
	if (streaminfos.size() != 1) {
		return NULL;
	}
	else {
		return streaminfos[0];
	}
}