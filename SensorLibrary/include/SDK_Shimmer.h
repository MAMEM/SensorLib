#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#if (_MSC_VER)
#pragma pack(push)
#pragma pack(1)
#else
#pragma option -a1
#endif

/// <summary>
///	Communication handle.
/// </summary>
typedef UINT					COMMUNICATION_HANDLE;
typedef COMMUNICATION_HANDLE*	PCOMMUNICATION_HANDLE;
/// <summary>
///	typedef returned error.
/// </summary>
typedef UINT IOError;

/// <summary>
/// Acquisition stopped.
/// </summary>
#define IDLE 0
/// <summary>
/// Normal acquisition.
/// </summary>
#define ACQUISITION 1

#define IO_NO_ERROR 0
/// <summary>
///  Generic Error .
/// </summary>
#define IO_ERR_GENERIC  0xC00B0000
/// <summary>
///  The param is out of range.
/// </summary>
#define IO_ERR_PARAMS_OUT_OF_RANGE 0xC00B0001
/// <summary>
///  The argument is null
/// </summary>
#define IO_ERR_NULL_ARGUMENT 0xC00B0002
/// <summary>
///  Invalid Acquisition Mode
/// </summary>
#define IO_ERR_INVALID_ACQUISITION_MODE 0xC00B0003
/// <summary>
///  Invalid operation.
/// </summary>
#define IO_ERR_INVALID_OPERATION 0xC00B0006
/// <summary>
///  Invalid operation.
/// </summary>
#define IO_ERR_OUT_OF_MEMORY 0xC00B0007
/// <summary>
///  Generic exception.
/// </summary>
#define IO_ERR_GENERIC_EXCEPTION 0xC00B0008
/// <summary>
///  Invalid IpAddress.
/// </summary>
#define IO_ERR_INVALID_IPADDRESS 0xC00B0009
/// <summary>
///  Device is already connected.
/// </summary>
#define IO_ERR_ALREADY_CONNECTED 0xC00B000A
/// <summary>
///  Invalid communication handle.
/// </summary>
#define IO_ERR_INVALID_HANDLE 0xC00B000D
/// <summary>
///  Hardware Error: device isn't responding ( device timeout ) .
/// </summary>
#define IO_ERR_CMD_TIMEOUT 0xC0090002
/// <summary>
///  Hardware Error: device communication error.
/// </summary>
#define IO_ERR_DEVICE_COMMUNICATION 0xC0090003 
/// <summary>
///  Hardware Error: device disconnected.
/// </summary>
#define IO_ERR_DEVICE_NOT_CONNECTED 0xC0090004 
/// <summary>
///  Cannont find acquisition device.
/// </summary>
#define IO_ERR_DEVICE_NOT_FOUND 0xC0090011 
/// <summary>
/// </summary>
/// <param name="context">Context pointer (application dependent).</param>
/// <param name="timespan">Acquisition time.</param>
/// <param name="heartRate">Heart rate value.</param>
/// <param name="gsr_value">GSR value.</param>
typedef void (__cdecl *DTACALLBACK)( void *context, const double acquisitionTime, const float heartRate, const float gsr_value );

/// <summary>
/// This function open the new handle for the multiple communication and try to connect the amplifier.
/// </summary>
/// <param name="COM">
/// Unicode string with device address.
/// </param>
/// <param name="pHandle">
/// Pointer Communication handle
/// </param>
/// <returns>Error Code.</returns>
__declspec(dllexport) IOError __stdcall Shimmer_OpenW( LPCWSTR COM, PCOMMUNICATION_HANDLE pHandle );
/// <summary>
/// This function open the new handle for the multiple communication and try to connect the amplifier.
/// </summary>
/// <param name="pHandle">
/// Pointer Communication handle
/// </param>
/// <param name="COM">String with device address.</param>
/// <returns>Error Code.</returns>
__declspec(dllexport) IOError __stdcall Shimmer_OpenA( LPCSTR COM, PCOMMUNICATION_HANDLE pHandle );
/// <summary>
/// Close hardware communication.
/// </summary>
/// <param name="Handle">
/// Communication handle
/// </param>
/// <returns>Error Code.</returns>
__declspec(dllexport) IOError __stdcall Shimmer_Close( COMMUNICATION_HANDLE Handle );
/// <summary>
/// Set acquisition callback.
/// </summary>
/// <param name="Handle">
/// Communication handle
/// </param>
/// <param name="DataCallBack">Pointer of function <see cref="DTACALLBACK"/> </param>
/// <param name="DataContext">Context pointer (application dependent).</param>
/// <returns>Error Code.</returns>
__declspec(dllexport) IOError __stdcall Shimmer_SetAcquisitionCallBack(  COMMUNICATION_HANDLE Handle,DTACALLBACK DataCallBack,	PVOID DataContext );
/// <summary>
/// This command sets the collection mode.
///
/// Sampling : standard collection mode.
///
/// Idle : data are being collected, but no more sent to the client
///
/// </summary>
/// <param name="Handle">
/// Communication handle
/// </param>
/// <param name="mode">Acquisition mode</param>
/// <returns>Error Code.</returns>
__declspec(dllexport) IOError __stdcall  Shimmer_SetAcquisitionMode( COMMUNICATION_HANDLE Handle, DWORD mode );

#ifdef UNICODE
#define Shimmer_Open  Shimmer_OpenW
#else
#define Shimmer_Open  Shimmer_OpenA
#endif // !UNICODE


#if (_MSC_VER)
#pragma pack(pop)
#else
#pragma option -a
#endif

#ifdef __cplusplus
}
#endif


