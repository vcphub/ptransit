// File: routes.cpp
// Description: File contains class Route member function definitions.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include "route.h"
#include "utils.h"

using namespace std;

// Global objects
RouteContainer RoutesList;
extern ofstream ferr;
extern ofstream fwarn;

// Static objects
int Route::route_count = 0;

// Route Constructor
Route::Route()
{
	ostringstream ss;

	Route::route_count++;
	ss << "r" << route_count;

	this->route_id = ss.str();
	this->stop_count = 0;
	this->distance = 0.0;
	assert(this->depot_name.length() == 0);
}

// Sort container start_time_list.
void Route::sort_start_times()
{
	// sort start_time_list
	sort(start_time_list.begin(), start_time_list.end());
}

// Given short name and bus id, find matching route object.
// Arguments: route short name, route bus id
// Return value: Route object pointer
Route * find_route(string short_name, string bus_id)
{
	Route * route = NULL; // desired route

	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		if(string_compare((*iter)->short_name, short_name) && 
						string_compare((*iter)->bus_id, bus_id))  
		{
			route = *iter;
			break;
		}
	}

	return route;
}

// Check single line of routes data file.
void check_route_tokenlist(string& filename, int linecnt, vector<string> tokenlist)
{
	// Errors
	if(tokenlist[0].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing route name."<<endl;
	if(tokenlist[1].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing bus id."<<endl;
	if(tokenlist[2].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing bus stop name."<<endl;

	// Warnings 
	if(tokenlist[3].length() == 0)	
		fwarn<<filename<<": line "<<linecnt<<" missing stage number."<<endl;
	if(tokenlist[4].length() == 0)	
		fwarn<<filename<<": line "<<linecnt<<" missing stop sequence."<<endl;
	if(tokenlist[5].length() == 0)	
		fwarn<<filename<<": line "<<linecnt<<" missing UP direction."<<endl;
	
}

int count_shuttle_routes()
{
	int count = 0;
	string s1 = "Shuttal";
	string s2 = "Shuttle";
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {
		if(string_compare((*iter)->short_name, s1)) {
			count++;
		} else if(string_compare((*iter)->short_name, s2)) {
			count++;
		}
	}
	return count;
}

// Read routes data file in CSV format.
// “route-number”, “bus-id”, “stop-name”, “stage-number”, “stop-sequence”, “up-direction”
void read_routes_file(string filename)
{
	ifstream fin(filename.c_str());
	int linecnt = 0;
	string buffer;
	string last_route_name = "";
	string last_bus_id = "";
	Route * route = NULL;

	cout<<"Reading routes data file "<<filename<<" ... "<<endl;
	getline(fin, buffer);

	while(!fin.eof())
	{
		size_t prev_pos = 0, curr_pos = 0;
		vector<string> tokenlist;
		string token; 

		linecnt++;
		// check string
		assert(buffer.length() != 0);
		
		// tokenize string buffer.
		curr_pos = buffer.find(',', prev_pos);

		while(1) {

			if(curr_pos == string::npos)
				curr_pos = buffer.length();

			// could be zero
			int token_length = curr_pos-prev_pos;

			// create new token and add it to tokenlist.
			token = buffer.substr(prev_pos, token_length);
			tokenlist.push_back(token);

			// reached end of the line
			if(curr_pos == buffer.length())
				break;

			prev_pos = curr_pos+1;
			curr_pos = buffer.find(',', prev_pos);
		} 

		// check all tokens.
		check_route_tokenlist(filename, linecnt, tokenlist);

		// Use 'tokenlist' to create and populate Route object
		if(string_compare(last_route_name, tokenlist[0]) && 
			string_compare(last_bus_id, tokenlist[1])) {

			// Add stop name to existing route object
			assert(route != NULL);
			route->stop_list.push_back(tokenlist[2]);

		} else {
			// Create new route
			route = new Route();
			route->short_name = tokenlist[0];
			route->bus_id = tokenlist[1];
			route->stop_list.push_back(tokenlist[2]);

			// Add object to global array.
			RoutesList.push_back(route);

			last_route_name = route->short_name;
			last_bus_id = route->bus_id;
		}

		// get next line
		tokenlist.clear();
		getline(fin, buffer);
	}

	cout << "Total lines read = " << linecnt << endl;
	cout << "Shuttle routes = " << count_shuttle_routes() << endl;
	cout << "Total routes = " << RoutesList.size() << endl << endl;
}


