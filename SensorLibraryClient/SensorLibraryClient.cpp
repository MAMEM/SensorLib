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
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t << ".xdf";
	//lib->startRecording(oss.str());
	lib->startRecording("C:/lalakis/test.xdf");
	std::cout << "Start recording 1" << std::endl;
	system("pause");
	lib->stopRecording();
	system("pause");
	return 0;
}
