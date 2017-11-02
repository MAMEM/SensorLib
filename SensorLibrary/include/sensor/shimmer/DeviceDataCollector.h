#pragma once

#include <stdlib.h>
#include <windows.h>
#include <vector>

#include "SDK_Shimmer.h"


namespace lsl
{
	class stream_outlet;
}



/// <summary>
/// Class unmanaged to use beplusltm sdk library.
/// </summary>
class DeviceDataCollector
{
	
public:
	DeviceDataCollector(void);
	~DeviceDataCollector(void);
	/// <summary>
	/// Open hardware communication
	/// </summary>
	unsigned int Open( std::string ipaddress );
	/// <summary>
	/// Close comminucation.
	/// </summary>
	unsigned int Close( );
	/// <summary>
	/// Set acquisition mode.
	/// </summary>
	unsigned int SetAcquisitionMode( unsigned int mode );

	
private:

	std::vector<std::vector<float>> _mychunk;
	unsigned int _lastError;
	lsl::stream_outlet* _outlet;
	COMMUNICATION_HANDLE _handle;
	std::string _comPort;
	int _index;


	static void __cdecl ProcData( void *context, const  double acquisitionTime, const float heartRate, const float gsr_value );
};


