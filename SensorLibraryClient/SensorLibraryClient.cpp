#include "stdafx.h"  
#include <iostream>  
#include "SensorLibrary.h"
using namespace std;
using namespace SensorLib;
int main()
{
	/*
	double a = 7.4;
	int b = 99;

	cout << "a + b = " <<
		SensorLibrary::Add(a, b) << endl;
	cout << "a * b = " <<
		SensorLibrary::Multiply(a, b) << endl;
	cout << "a + (a * b) = " <<
		SensorLibrary::AddMultiply(a, b) << endl;

	EmotivSensor* insight = new EmotivSensor();
	insight->connect();
	system("pause");
	insight->disconnect();
	std::cout << "disconnected from device " << std::endl;
	system("pause");
	*/
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
