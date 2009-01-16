
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <cassert>

#include "stop.h"
#include "route.h"

using namespace std;

int Stop::stop_count = 0;

Stop::Stop(string stop_name) 
{
	ostringstream ss; 
	Stop::stop_count++; 

	ss << "s" << stop_count; // generates unique key
	this->stop_id = ss.str(); 
	this->stop_name = stop_name;
}

ofstream fstop("temp.txt");
void Stop::print_trip_times()
{
	fstop << stop_name <<endl;
	for(size_t i = 0; i < route_list.size(); i++) {
		fstop << route_list[i]->short_name <<" (";
		fstop << route_list[i]->bus_id << "), ";
	}
	fstop<<endl<<endl;
}

// Print stop object pointer.
ostream& operator<<(ostream& os, Stop * ps)
{
	os << ps->stop_name;
	return os;
}
