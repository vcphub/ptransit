
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

// Check input data for errors and inconsistencies.
void check_data()
{
	ofstream fout("check.txt");

	Route * route = NULL;	
	int error_cnt = 0, total_cnt = 0;
	// For each object in RoutesList.
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {
		route = (*iter);

		int master_cnt = route->stop_list.size();
		int depot_cnt =  route->stop_count;

		if(route->stop_count == 0) continue;

		if(route->stop_count != (int)route->stop_list.size()) {
			// Data mismatch
			fout<<"Route = "<<setw(6)<<route->short_name;
			fout<<", Bus id = "<<setw(4)<<route->bus_id; 
			fout<<", master = "<<setw(4)<<route->stop_list.size();
			fout<<", depot = "<<setw(4)<<route->stop_count;
			fout<<", diff = "<<setw(4)<<(master_cnt-depot_cnt)<<endl;
			error_cnt++;
		}
		total_cnt++;
	}
	fout << endl;
	fout << "Total routes = " << total_cnt << endl;
	fout << "Mismatch count = " << error_cnt << endl;

	fout.close();
}

