#pragma once

#include <WinBase.h>
#include <iostream>
#include <string>
using std::string;
using std::cerr;

LARGE_INTEGER operator-(const LARGE_INTEGER &lval, const LARGE_INTEGER &rval) {
	LARGE_INTEGER tmp;
	tmp.QuadPart = lval.QuadPart-rval.QuadPart;
	return tmp;
}


class Profiler {
	LARGE_INTEGER	start_time;
	LARGE_INTEGER	stop_time;
	LARGE_INTEGER	destroy_time;
	LARGE_INTEGER	freq;
	string			profiler;
	double toSeconds(LARGE_INTEGER& ticks) { return ((double)ticks.QuadPart)/freq.QuadPart; }
	void init() { 
		QueryPerformanceCounter(&start_time);
		QueryPerformanceFrequency(&freq);
		stop_time.QuadPart=destroy_time.QuadPart = 0;
	}

public:
	Profiler(const string& name): profiler(name) { init(); }

	Profiler(): profiler("Profiler") { init(); }

	void stop() {
		QueryPerformanceCounter(&stop_time);
		cerr << profiler << " was stopped after " << toSeconds(stop_time - start_time) << " seconds \n";
	}

	~Profiler() {
		QueryPerformanceCounter(&destroy_time);
		double ttl = toSeconds(destroy_time-start_time);
//		cerr << profiler << " was stopped after " << toSeconds(stop_time - start_time) << " seconds \n";
		cerr << profiler << " lived " << ttl << " seconds \n";
	}

};

