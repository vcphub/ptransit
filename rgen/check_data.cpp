
// File: check_data.cpp
// Description: Contains functions to check input data for errors 
// and inconsistencies.

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cassert>
#include "route.h"

using namespace std;

extern RouteContainer RoutesList;
extern RouteContainer MissingRoutesList;
extern ofstream ferr;

// Process/sort the data before generating any output.
void process_data()
{
	Route * route = NULL;	

	// For each object in RoutesList.
	cout<<"Sort start times ... ";
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {
		route = (*iter);
		route->sort_start_times();
	}
	cout<<"Done."<<endl;
}

// Description: Check input data for errors and inconsistencies.
// Generate systematic error report. 
// Part 1 : Missing routes
// Part 2 : Bus stop count mismatch
// Part 3 : Unused routes
// Part 4 : Routes with multiple depots
void check_data()
{
	Route * route = NULL;	
	RouteIterator iter;

	cout<<"Checking routes data for inconsistencies ...";

	ferr<<endl<<endl;
	ferr<<"# Following routes are not present in master sheet. "<<endl<<endl;
	iter = MissingRoutesList.begin();
	for(; iter != MissingRoutesList.end(); iter++) {
		route = (*iter);

		ferr<<"Depot = "<<setw(16);
		for(size_t i = 0; i < route->depot_list.size(); i++)
			ferr<<route->depot_list[i]<<", ";

		ferr<<" Route = "<<setw(10)<<route->short_name;
		ferr<<",  Bus id = "<<setw(4)<<route->bus_id << endl; 
	}
	ferr << endl;
	ferr << "# Total missing routes = " << MissingRoutesList.size() << endl;
	ferr << "----------------------------------------------" << endl;

	ferr<<endl<<endl;
	ferr<<"# Following routes have bus stops count mismatch. "<<endl<<endl;
	// For each object in RoutesList.
	int error_cnt = 0, total_cnt = 0;
	iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {
		route = (*iter);

		int master_cnt = route->stop_list.size();
		int depot_cnt =  route->stop_count;

		if(route->stop_count == 0)
			continue;

		// check bus stop count, from master sheet and trips sheet.
		if(route->stop_count != (int)route->stop_list.size()) {
			ferr<<"Route = "<<setw(6)<<route->short_name;
			ferr<<",  Bus id = "<<setw(4)<<route->bus_id; 
			ferr<<",  master = "<<setw(4)<<route->stop_list.size();
			ferr<<",  depot = "<<setw(4)<<route->stop_count;
			ferr<<",  diff = "<<setw(4)<<(master_cnt-depot_cnt)<<endl;
			error_cnt++;
		}
		total_cnt++;
	}
	ferr << endl;
	ferr << "# Total active routes = " << total_cnt << endl;
	ferr << "# Total routes with bus stop count mismatch = " << error_cnt << endl;
	ferr << "----------------------------------------------" << endl;

	// Check for unused routes
	ferr<<endl<<endl;
	ferr<<"# Following routes from master sheet are not used by any depot (why?). ";
	ferr<<endl<<endl;
	// For each object in RoutesList.
	iter = RoutesList.begin();
	int unused_count = 0;
	for(; iter != RoutesList.end(); iter++) {
		route = (*iter);

		// Number of bus stops as per depot data.
		int depot_bus_cnt = route->stop_count;
		// Number of trips read.
		size_t start_times_cnt = route->start_time_list.size();

		if((depot_bus_cnt == 0) || (start_times_cnt == 0)) {
			ferr<<"Route = "<<setw(6)<<route->short_name;
			ferr<<",  Bus id = "<<setw(2)<<route->bus_id << endl; 
			unused_count++;
			continue;
		}
	}

	ferr << endl;
	ferr << "# Total unused routes = " << unused_count << endl;
	ferr << "----------------------------------------------" << endl;

	// Check for routes with multiple depots.
	ferr<<endl<<endl;
	ferr<<"# Following routes are operated by more than one depots.";
	ferr<<endl<<endl;
	// For each object in RoutesList.
	iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {
		route = (*iter);
		if(route->depot_list.size() > 1) {
			ferr<<"Route = "<<setw(8)<<route->short_name;
			ferr<<",  Bus id = "<<setw(5)<<route->bus_id;
			ferr<<",  Depots = ";
			for(size_t i = 0; i < route->depot_list.size(); i++) 
				ferr<<route->depot_list[i]<<",  ";
			ferr<<endl;
		}
	}
	ferr << endl;
	// TODO : count
	ferr << "----------------------------------------------" << endl;

	cout << " Done."<<endl;
}

