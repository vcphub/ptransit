
// File: check_data.cpp
// Description: Contains functions to process and check input data 
// for errors and inconsistencies.

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cassert>
#include <cstdlib>
#include "route.h"

using namespace std;

extern TripGroupContainer TripGroupsList;
extern TripGroupContainer MissingTripGroupsList;
extern ofstream ferr;

/* 
 * Process/sort the data before generating any output.
 */
void process_data()
{
	TripGroup * tripgroup = NULL;	

	// For each object in TripGroupsList.
	cout<<"Sort start times ... ";
	TripGroupIterator iter = TripGroupsList.begin();
	for(; iter != TripGroupsList.end(); iter++) {
		tripgroup = (*iter);
		tripgroup->sort_start_times();

		int stop_count = tripgroup->stop_list.size();
		if(stop_count <= 1) {
                        cout << "Error : Invaid number of stops. ";
			cout << tripgroup->route->short_name << " " << tripgroup->bus_id << endl;
			exit(-1);
		}
		// Calculate interval in mins.
		tripgroup->interval = (double)tripgroup->estimated_time/(double)(stop_count-1); 
	}
	cout<<"Done."<<endl;
}

/* 
 * Description: Check input data for errors and inconsistencies.
 * Generate systematic error report. 
 * Part 1 : Missing tripgroups.
 * Part 2 : Bus stop count mismatch.
 * Part 3 : Unused tripgroups.
 * Part 4 : TripGroups with multiple depots.
 */
void check_data()
{
	TripGroup * tripgroup = NULL;	
	TripGroupIterator iter;

	cout<<"Checking tripgroup data for inconsistencies ...";

	ferr<<endl<<endl;
	ferr<<"# Following tripgroups are not present in master sheet. "<<endl<<endl;
	iter = MissingTripGroupsList.begin();
	for(; iter != MissingTripGroupsList.end(); iter++) {
		tripgroup = (*iter);

		ferr<<"Depot = "<<setw(16);
		for(size_t i = 0; i < tripgroup->depot_list.size(); i++)
			ferr<<tripgroup->depot_list[i]<<", ";

		ferr<<" TripGroup = "<<setw(10)<<tripgroup->route->short_name;
		ferr<<",  Bus id = "<<setw(4)<<tripgroup->bus_id << endl; 
	}
	ferr << endl;
	ferr << "# Total missing tripgroups = " << MissingTripGroupsList.size() << endl;
	ferr << "----------------------------------------------" << endl;

	ferr<<endl<<endl;
	ferr<<"# Following tripgroups have bus stops count mismatch. "<<endl<<endl;
	// For each object in TripGroupsList.
	int error_cnt = 0, total_cnt = 0;
	iter = TripGroupsList.begin();
	for(; iter != TripGroupsList.end(); iter++) {
		tripgroup = (*iter);

		int master_cnt = tripgroup->stop_list.size();
		int depot_cnt = tripgroup->stop_count;

		if(tripgroup->stop_count == 0)
			continue;

		// check bus stop count, from master sheet and trips sheet.
		if(tripgroup->stop_count != (int)tripgroup->stop_list.size()) {
			ferr<<"TripGroup = "<<setw(6)<<tripgroup->route->short_name;
			ferr<<",  Bus id = "<<setw(4)<<tripgroup->bus_id; 
			ferr<<",  master = "<<setw(4)<<tripgroup->stop_list.size();
			ferr<<",  depot = "<<setw(4)<<tripgroup->stop_count;
			ferr<<",  diff = "<<setw(4)<<(master_cnt-depot_cnt)<<endl;
			error_cnt++;
		}
		total_cnt++;
	}
	ferr << endl;
	ferr << "# Total active tripgroups = " << total_cnt << endl;
	ferr << "# Total tripgroups with bus stop count mismatch = " << error_cnt << endl;
	ferr << "----------------------------------------------" << endl;

	// Check for unused tripgroups.
	ferr<<endl<<endl;
	ferr<<"# Following tripgroups from master sheet are not used by any depot (why?). ";
	ferr<<endl<<endl;
	// For each object in TripGroupsList.
	int unused_count = 0;
	iter = TripGroupsList.begin();
	for(; iter != TripGroupsList.end(); iter++) {
		tripgroup = (*iter);

		// Number of bus stops as per depot data.
		int depot_bus_cnt = tripgroup->stop_count;
		// Number of trips read.
		size_t start_times_cnt = tripgroup->start_time_list.size();

		if((depot_bus_cnt == 0) || (start_times_cnt == 0)) {
			ferr<<"TripGroup = "<<setw(6)<<tripgroup->route->short_name;
			ferr<<",  Bus id = "<<setw(2)<<tripgroup->bus_id << endl; 
			unused_count++;
			continue;
		}
	}

	ferr << endl;
	ferr << "# Total unused tripgroups = " << unused_count << endl;
	ferr << "----------------------------------------------" << endl;

	// Check for tripgroups with multiple depots.
	ferr<<endl<<endl;
	ferr<<"# Following tripgroups are operated by more than one depots.";
	ferr<<endl<<endl;
	// For each object in TripGroupsList.
	iter = TripGroupsList.begin();
	for(; iter != TripGroupsList.end(); iter++) {
		tripgroup = (*iter);
		if(tripgroup->depot_list.size() > 1) {
			ferr<<"TripGroup = "<<setw(8)<<tripgroup->route->short_name;
			ferr<<",  Bus id = "<<setw(5)<<tripgroup->bus_id;
			ferr<<",  Depots = ";
			for(size_t i = 0; i < tripgroup->depot_list.size(); i++) 
				ferr<<tripgroup->depot_list[i]<<",  ";
			ferr<<endl;
		}
	}
	ferr << endl;
	// TODO : count
	ferr << "----------------------------------------------" << endl;

	cout << " Done."<<endl;
}

