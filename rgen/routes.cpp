// File: routes.cpp
// Description: File containers class Route member function definitions.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include "route.h"
#include "utils.h"

using namespace std;

// Global objects
RouteContainer RoutesList;
// Static objects
int Route::route_count = 0;

// Constructor
Route::Route()
{
	ostringstream ss;

	Route::route_count++;
	ss << "r" << route_count;

	this->route_id = ss.str();
	this->stop_count = 0;
	this->distance = 0.0;
}

// Given short name and bus id, find matching route object.
// Arguments: route short name, route bus id
// Return value: Route object pointer
Route * find_route(string short_name, string bus_id)
{
	Route * route = NULL; // desired route

	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		if(string_compare((*iter)->short_name, short_name) && string_compare((*iter)->bus_id, bus_id))  
		{
			route = *iter;
			break;
		}
	}

	if(route == NULL) {
		cout << "Warning: could not find route object, "<< short_name <<", "<< bus_id<<endl;
	}

	return route;
}

// Description: Read trips data file in CSV format.
// “route-number”, “bus-id”, “no-of-stops”, “distance”, “estimated-time”, “start-times”
// Arguments: trip data file name
// Return value: void
void read_trips_file(string filename)
{
	ifstream fin(filename.c_str());
	int linecnt = 0;
	
	cout<<"Reading trips data file "<<filename<<" ... "<<endl;
	string buffer;
	getline(fin, buffer);
	linecnt++;
	while(!fin.eof())
	{
		size_t prev_pos = 0, curr_pos = 0;
		vector<string> tokenlist;
		string token;
		Route * route = NULL;

		// check string
		assert(buffer.length() != 0);

		//cout << linecnt << " " << buffer << endl;
		
		// tokenize line string
		curr_pos = buffer.find_first_of(',', prev_pos);

		while(1) {

			if(curr_pos == string::npos)
				curr_pos = buffer.length();

			// It is possible to get empty token
			token = buffer.substr(prev_pos, curr_pos-prev_pos);
			tokenlist.push_back(token);

			if(curr_pos == buffer.length())
				break;

			prev_pos = curr_pos+1;
			curr_pos = buffer.find_first_of(',', prev_pos);
		} 

		// Use token list to find and update Route object
		route = NULL;
		// Use short name and bus id to find route.
		route = find_route(tokenlist[0], tokenlist[1]);

		if(route != NULL) {
			//  Correct Route found.
			
			route->stop_count = atoi(tokenlist[2].c_str());
			route->distance = atof(tokenlist[3].c_str());
			route->estimated_time = atoi(tokenlist[4].c_str());

			// Read start times
			for(size_t i = 5; i < tokenlist.size(); i++) {
				int start_time = 0;

				// ignore empty token because it is useless.
				if(tokenlist[i].length() == 0)
					continue;

				start_time = time_string_to_mins(tokenlist[i]);
				route->start_time_list.push_back(start_time);
			}
		}

		// get next line
		getline(fin, buffer);
		linecnt++;
	}

	cout << "Total lines read = " << linecnt << endl << endl;
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
	linecnt++;

	while(!fin.eof())
	{
		size_t prev_pos = 0, curr_pos = 0;
		vector<string> tokenlist;
		string token; 

		// check string
		assert(buffer.length() != 0);
		//cout << linecnt << " " << buffer << endl;
		
		// tokenize string buffer.
		curr_pos = buffer.find(',', prev_pos);

		while(1) {

			if(curr_pos == string::npos)
				curr_pos = buffer.length();

			int token_length = curr_pos-prev_pos;

			if(token_length == 0) 
				cout << "Warning: line " << linecnt << " missing cell value."<<endl;

			// create new token and add it to tokenlist.
			token = buffer.substr(prev_pos, token_length);
			tokenlist.push_back(token);

			// reached end of the line
			if(curr_pos == buffer.length())
				break;

			prev_pos = curr_pos+1;
			curr_pos = buffer.find(',', prev_pos);
		} 

		// Use token list to create and populate Route object
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
		linecnt++;
	}

	cout << "Total lines read = " << linecnt << endl;
	cout << "Total routes = " << RoutesList.size() << endl << endl;
}


