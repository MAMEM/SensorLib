#include "stdafx.h"
#include <sensor/shimmer/DeviceDataCollector.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>


#pragma unmanaged
#include "lsl_cpp.h"
#pragma managed

#define Channel_Count  2
//#define SamplingRate	10.2
#define SamplingRate	256
#define PacketLSL 64


DeviceDataCollector::DeviceDataCollector(void)
	:_mychunk( Channel_Count, std::vector<float>( PacketLSL ) )
{
	_lastError = 0;
	_handle = NULL;
	_outlet = NULL;
	_index = 0;
}

DeviceDataCollector::~DeviceDataCollector(void)
{
	if( NULL != _outlet )
	{
		delete _outlet;
		_outlet = NULL;
	}
	if( NULL != _handle )
	{
		_handle = NULL;
	}
}
unsigned int DeviceDataCollector::Open( std::wstring comPort )
{
	//_comPort = comPort;
	//std::cout << comPort << std::endl;
	//std::wstring stemp = std::wstring(comPort.begin(), comPort.end());
	unsigned int error;
	try {
		error = Shimmer_Open(comPort.c_str(), &_handle);
	}
	catch(...){
		error = 0xC00B0000;
	}
	//unsigned int error = Shimmer_Open(L"COM3", &_handle);
	if( error == IO_NO_ERROR )
	{

		Shimmer_SetAcquisitionCallBack( _handle, ( DTACALLBACK ) DeviceDataCollector::ProcData,  this );
	
	
		lsl::stream_info info("Shimmer","BIO", Channel_Count,SamplingRate,lsl::cf_float32,"Shimmer_" + _comPort);

		lsl::xml_element time = info.desc().append_child("event_synchronization");
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
		lslclock << lsl::lsl_local_clock();
		//offset in milliseconds
		std::ostringstream timeoffset;
		timeoffset.precision(15);
		timeoffset << (ms.count() - lsl::lsl_local_clock());
		time.append_child_value("unixtimestamp", unixtimestamp.str());
		time.append_child_value("unixtimestmapms", unixtimestampms.str());
		time.append_child_value("lslclock", lslclock.str());
		time.append_child_value("streamoffset", timeoffset.str());
		// add some description fields
		info.desc().append_child_value("manufacturer", "Shimmer");


		info.desc().append_child("channel")
			.append_child_value("name","Heart Rate")
			.append_child_value("unit","bpm")
			.append_child_value("type","BIO SIGNAL");

		info.desc().append_child("channel")
			.append_child_value("name","GSR")
			.append_child_value("unit","kOhm")
			.append_child_value("type","BIO SIGNAL");


		// make a new outlet
		_outlet = new lsl::stream_outlet (info);
	}

	return error;
}
unsigned int DeviceDataCollector::Close( )
{
	return Shimmer_Close( _handle );
}


unsigned int DeviceDataCollector::SetAcquisitionMode( unsigned int mode )
{
	return Shimmer_SetAcquisitionMode( _handle, mode );
}

void __cdecl DeviceDataCollector::ProcData( void *context, const  double acquisitionTime, const float heartRate, const float gsr_value )
{
	std::cout << "procdata" << std::endl;
	((DeviceDataCollector*)context)->_mychunk[ 0 ][ ((DeviceDataCollector*)context)->_index] = (float)heartRate;
	((DeviceDataCollector*)context)->_mychunk[ 1 ][ ((DeviceDataCollector*)context)->_index] = (float)gsr_value;
	((DeviceDataCollector*)context)->_index++;

	if( ((DeviceDataCollector*)context)->_index == PacketLSL)
	{
		std::vector<std::vector<float>> chunk( PacketLSL );
					
		for( int i = 0; i < PacketLSL; i++ )
		{
			std::cout << ((DeviceDataCollector*)context)->_mychunk[0][i] << std::endl;
			chunk[ i ].push_back(((DeviceDataCollector*)context)->_mychunk[ 0 ][ i ] );
			chunk[ i ].push_back(((DeviceDataCollector*)context)->_mychunk[ 1 ][ i ] );
		}
		((DeviceDataCollector*)context)->_outlet->push_chunk(chunk);
		((DeviceDataCollector*)context)->_index = 0;
	}
	
}

