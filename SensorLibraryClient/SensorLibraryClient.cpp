#include "stdafx.h"  
#include <iostream>  
#include "SensorLibrary.h"
using namespace std;
using namespace SensorLib;
int main()
{
	SensorLibrary *lib = new SensorLibrary();
	system("pause");
	lib->printStatus();
	system("pause");
	lib->startRecording("test.xdf");
	system("pause");
	lib->stopRecording();
	system("pause");
	return 0;
}
