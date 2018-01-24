#include "stdafx.h"  
#include <iostream>  
#include <ostream>
#include <sstream>
#include <ctime>
#include "SensorLibrary.h"
using namespace std;
using namespace SensorLib;
int main()
{
	SensorLibrary *lib = new SensorLibrary();
	system("pause");
	lib->printStatus();
	system("pause");
	cout << "stopping recording" << endl;
	lib->stopRecording();
	cout << "recording stopped" << endl;
	system("pause");
	cout << "starting recording" << endl;
	lib->startRecording();
	system("pause");
	cout << "stopping recording again" << endl;
	lib->stopRecording();
	cout << "recording stopped again" << endl;
	/*
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t << ".xdf";
	lib->startRecording();
	std::cout << "Start recording 1" << std::endl;
	system("pause");
	lib->stopRecording();
	system("pause");
	lib->startRecording();
	system("pause");
	lib->stopRecording();
	system("pause");
	*/
	system("pause");
	return 0;
}