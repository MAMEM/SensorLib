#include "stdafx.h"  
#include <iostream>  
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
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t << ".xdf";
	lib->startRecording(oss.str());
	std::cout << "Start recording 1" << std::endl;
	system("pause");
	lib->stopRecording();
	system("pause");
	t = std::time(0);
	oss.str("");
	oss.clear();
	oss << t << ".xdf";
	lib->startRecording(oss.str());
	std::cout << "Start recording 2" << std::endl;
	system("pause");
	lib->stopRecording();
	system("pause");
	return 0;
}
