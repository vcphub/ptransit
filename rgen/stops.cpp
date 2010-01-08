// File: stops.cpp
// Description: Container member function definitions for class Stop.
// Also functions for generating output HTML.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <cassert>
#include <cstdlib>

#include "stop.h"
#include "route.h"
#include "output_html.h"

using namespace std;

extern ofstream ferr;
int Stop::stop_count = 0;

// Constructor
Stop::Stop(string stop_name) 
{
	ostringstream ss; 
	Stop::stop_count++; 

	ss << "s" << stop_count; // generates unique key
	this->stop_id = ss.str(); 
	this->stop_name = stop_name;
}

// Description: For a given pair of bus stop object and tripgroup object, 
// print all major bus stop on the tripgroup, after given bus stop is 
// passed. First and last bus stop on the tripgroup is always printed.
void print_major_stops(ofstream& fout, Stop * stop, TripGroup * tripgroup)
{
	int stop_count = tripgroup->stop_list.size();
	bool curr_stop_passed = false;
	int threshold = 40; // to be tuned
	string dot_string;
	vector<int> index_list;
	int curr_stop_index = tripgroup->get_stop_index(stop);

	// Always print first bus stop
	fout<<"<table border=1px bordercolor=gray cellpadding=2px cellspacing=0px >" << endl;

	fout<<"<tr align='center' style='font-family:Verdana; font-size:12px'>"<<endl;
	
	// For each bus stop on the tripgroup.
	for(int i = 0; i < stop_count; i++) {
		Stop * iter_stop = tripgroup->stop_list[i];

		if(iter_stop == stop) 
			curr_stop_passed = true;

		// Print row decision. Always print first, last and current bus stop.
		if((i == 0) || (i == stop_count-1) || (iter_stop == stop)) {

			if(!dot_string.empty()) {
				fout<<"\t<td >"<< dot_string <<"</td>"<<endl;
				dot_string.clear();
				index_list.push_back(-1); // no arrival time.
			}
			if(iter_stop == stop)
				fout<<"\t<th ><font color=\"#0000FF\">"<< iter_stop->stop_name <<"</font></th>"<<endl;
			else
				fout<<"\t<td >"<< iter_stop->stop_name <<"</td>"<<endl;

			int diff_index = tripgroup->get_stop_index(iter_stop) - curr_stop_index; 

			if(curr_stop_passed && (diff_index < 0)) {
				// report error
				ferr<< "Dupe: Two bus stop with same name present in a tripgroup." << endl;
				ferr<< tripgroup->route->short_name<<" "<< tripgroup->bus_id; 
				ferr<< iter_stop->stop_name<<" "<<stop->stop_name << endl;
				index_list.push_back(-1);

			} else if(curr_stop_passed) {
				index_list.push_back(diff_index); 
			} else
				index_list.push_back(-1); 

		// Logic to identify major stops.
		} else if(((int)iter_stop->tripgroup_list.size() > threshold) && (curr_stop_passed)) {

			if(!dot_string.empty()) {
				fout<<"\t<td >"<< dot_string <<"</th>"<<endl;
				dot_string.clear();
				index_list.push_back(-1); // no arrival time
			}

			fout<<"\t<td >"<< iter_stop->stop_name <<"</td>"<<endl;
			threshold += 10;

			int diff_index = tripgroup->get_stop_index(iter_stop) - curr_stop_index; 
			if(diff_index < 0) {
				ferr<< "Dupe: Two bus stop with same name present in a tripgroup." << endl;
				ferr<< tripgroup->route->short_name<<" "<< tripgroup->bus_id; 
				ferr<< iter_stop->stop_name<<" "<<stop->stop_name << endl;
				index_list.push_back(-1);
			} else
				index_list.push_back(diff_index);

		} else {
			dot_string.push_back('.');
		}
	} // next stop

	// Second row
	fout<<"<tr align='center' style='font-family:Verdana; font-size:12px'>"<<endl;
	for(size_t i = 0; i < index_list.size(); i++) {
		if(index_list[i] == -1)
			fout<<"\t<td >"<<"&nbsp;"<<"</td>"<<endl;
		else {
			int relative_time = index_list[i] * tripgroup->interval;
			if(relative_time < 0) {
				cout << index_list[i] <<" "<< tripgroup->interval << endl;
				exit(-1);
			}
			fout<<"\t<td >"<< relative_time <<" Mins"<<"</td>"<<endl;
		}
	}
	fout<<"</tr>"<<endl;
	fout<<endl;
	fout<<"</table>"<<endl;
}

// Generate HTML page showing schedule information for bus stop.
void Stop::print_trip_times()
{
	// Create output file name for this bus stop.
	ostringstream ss;
	ss<< "en-" << stop_id <<".html";
	string filename = "html/" + ss.str();
	ofstream fout;
	file_open_write(filename.c_str(), fout);

	//cout << "Generating file " << filename << endl;

	fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
	fout<<endl<<endl;
	fout<<"<html>"<<endl;
	//print_head(fout);
	fout<<"<body>"<<endl;

	// Print basic bus stop information.
	fout<<"<center><span style='font-size: 20px; font-weight:bold; \
			font-family:Verdana; color:blue'>";
	fout<<"PMPML Timetable<br />"<< endl;
	fout<<"Bus Stop = " << stop_name <<"<br />"<< endl;
	fout<<"</span></center>"<<endl;
	fout<<"<hr>"<<endl;

	// For each tripgroup in this stop object's tripgroup_list, print trip times
	for(size_t i = 0; i < tripgroup_list.size(); i++) {
		TripGroup * tripgroup = tripgroup_list[i];

		// Print basic tripgroup information.
		int stop_count = tripgroup->stop_list.size();
		fout<<"<h2> TripGroup " << tripgroup->route->short_name <<"</h2>"<<endl; 
		//fout<<"<h5> All days of week </h5>"<< endl;
		fout<<"<h4><b>Direction: "<< tripgroup->stop_list[0]<<"\tto\t";
		fout<< tripgroup->stop_list[stop_count-1] <<"</b></h4>"<< endl;

		// Calculate sequence number of this stop in the tripgroup.
		int stop_index = tripgroup->get_stop_index(this);

		// TODO: print major bus stops
		print_major_stops(fout, this, tripgroup);
		
		fout<<"<h5> Bus Arrival Times: </h5>"<< endl;

		// Create new table.
		// For each start time value, calculate stop visit time.
		fout << "<table border=1px bordercolor=gray cellpadding=4px cellspacing=0px >" << endl;

		int col_cnt = 15;
		if((int)tripgroup->start_time_list.size() < col_cnt)
			col_cnt = tripgroup->start_time_list.size();

		for(size_t trip_cnt = 0; trip_cnt < tripgroup->start_time_list.size(); trip_cnt++) {
			if((trip_cnt % col_cnt) == 0)		
				fout<<"<tr align='center' style='font-family:Verdana; font-size:12px'>"<<endl;

			double trip_visit_time = tripgroup->start_time_list[trip_cnt] + tripgroup->interval * stop_index;
			fout<<"\t<td nowrap=nowrap>"<< time_mins_to_hhmm((int)trip_visit_time) <<"</td>"<<endl;

			if(((trip_cnt+1) % col_cnt) == 0)		
				fout<<"</tr>"<<endl;
		}

		fout<<endl;
		fout<<"</table>"<<endl;
		fout<<"<br />"<<endl;
		fout<<"<hr>"<<endl;
	} // next tripgroup

	fout<<"</body>"<<endl;
	fout<<"</html>"<<endl;
	fout.close();
}

// Print stop object pointer.
ostream& operator<<(ostream& os, Stop * ps)
{
	os << ps->stop_name;
	return os;
}
