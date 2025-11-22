/*
	Credits to Professor Fox who provided us this class for a previous project in COP5303C
*/

#include "SimpleTimer.h"
#include <iostream>
using std::cout;
using std::endl;

SimpleTimer::SimpleTimer()
{
	start = highResClock::now();
}

SimpleTimer::~SimpleTimer()
{
	end = highResClock::now();
	elapsed = end - start;
	cout << "Elapsed time: " << elapsed.count() << " seconds" << endl << "------------------------------------------------------------" << endl;
}