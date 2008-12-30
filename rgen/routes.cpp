/*
 * Program Description: 
 * Read PMT routes data. Count total number of bus stops.
 *
 */


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "route.h"

using namespace std;

// Global objects
RouteContainer RoutesList;

// Print HTML page for each page
// Use interpolation
void print_html()
{
	ofstream fout("output.html");

	fout<<"<html>"<<endl;
	fout<<"<header>"<<endl;
	fout<<"<h2> PMPML Route </h2>"<< endl;
	fout<<"</header>"<<endl;
	fout<<"<body>"<<endl;

	// For each route 
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		Route * route = (*iter);
		int stop_count = route->stop_list.size();
		int trip_count = route->start_time_list.size();
		int interval = 0;

		// Interpolation
		if(stop_count == 0 || trip_count == 0)
			continue;

		interval = route->estimated_time/stop_count; // mins

		fout << "Route " << route->short_name << endl;
		fout << "<table border=\"1\">" << endl;
		// TODO: Use bus id
		
		// Print Header
		// For each stop print stop name
		fout<<"<tr>"<<endl;
		vector<string>::iterator stop_iter = route->stop_list.begin();
		for(; stop_iter != route->stop_list.end(); stop_iter++) 
			fout<<"<td>"<< (*stop_iter) << "</td>" << endl;
		fout<<"</tr>"<<endl;

		// For each trip	
		vector<int>::iterator st_iter = route->start_time_list.begin();
		for(; st_iter != route->start_time_list.end(); st_iter++) {
			
			fout<<"<tr>"<<endl;
			// For each stop
			vector<string>::iterator stop_iter = route->stop_list.begin();
			int time = (*st_iter);
			for(; stop_iter != route->stop_list.end(); stop_iter++) {
				fout<<"<td>"<< time <<"</td>";
				time += interval;
			}

			fout<<"</tr>"<<endl;
		}
		fout<<endl;
		fout<<"</table>"<<endl;
	}

	fout << "</body>" << endl;
	fout << "</html>" << endl;
}

// Utility Function
int time_string_to_mins(string input)
{
	int output;
	string str_hours, str_mins;
	int hours = 0, mins = 0;
	size_t prev_pos = 0, curr_pos = 0;

	curr_pos = input.find_first_of('.', 0);
	str_hours = input.substr(prev_pos, curr_pos-prev_pos);

	curr_pos += 1;
	str_mins = input.substr(curr_pos, input.size()-curr_pos);

	hours = atoi(str_hours.c_str());
	mins = atoi(str_mins.c_str());

	cout << "hours = "<< hours <<", mins = "<< mins << endl;
	output = hours * 60 + mins;

	return output;
}

// Given short name and bus id, find matching route object.
// Arguments: route short name, route bus id
// Return value: Route object pointer
Route * find_route(string short_name, string bus_id)
{
	Route * route = NULL;

	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		if(string_compare((*iter)->short_name, short_name) &&
			string_compare((*iter)->bus_id, bus_id))  {

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
// Arguments: trip data file name
// Return value: void
void read_trips_file(string filename)
{
	ifstream fin(filename.c_str());
	int linecnt = 0;
	
	string buffer;
	getline(fin, buffer);
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

		// Use token list to update Route object
		route = NULL;
		route = find_route(tokenlist[0], tokenlist[1]);

		if(route != NULL) {
			//  Correct Route found.
			
			route->estimated_time = atoi(tokenlist[4].c_str());
			// TODO: Check bus stop count.

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
		cout << "Tokens read = " << tokenlist.size() << endl;
	}

	cout << "Total lines read = " << linecnt << endl;

}

// Read routes data file in CSV format.
void read_routes_file(string filename)
{
	ifstream fin(filename.c_str());
	int linecnt = 0;
	string buffer;
	string last_route_name = "";
	string last_bus_id = "";
	Route * route = NULL;

	getline(fin, buffer);

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
				cout << "Warning: line " << linecnt << "missing cell value."<<endl;

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
		if((last_route_name == tokenlist[0]) && 
						(last_bus_id == tokenlist[1])) {

			// Add stop name to existing route object
			assert(route != NULL);
			route->stop_list.push_back(tokenlist[2]);

		} else {
			route = new Route();
			RoutesList.push_back(route);

			route->short_name = tokenlist[0];
			route->bus_id = tokenlist[1];

			last_route_name = route->short_name;
			last_bus_id = route->bus_id;
		}

		// get next line
		tokenlist.clear();
		getline(fin, buffer);
		linecnt++;
	}

	cout << "Total lines read = " << linecnt << endl;
	cout << "Total routes = " << RoutesList.size() << endl;
}

// Comparison function
bool string_compare(string& s1, string& s2)
{
	bool result = false;
	const char * cs1 = s1.c_str();
	const char * cs2 = s2.c_str();
	int li = 0, ri = 0;
	int ci = 0; 
	int lhs, rhs;
	char common[256];

	while(1) {
		lhs = cs1[li++];
		rhs = cs2[ri++];
		
		while(isalnum(lhs) == 0 && (lhs != '\0')) 
			lhs = cs1[li++];

		while(isalnum(rhs) == 0 && (rhs != '\0')) 
			rhs = cs2[ri++];

		lhs = tolower(lhs);
		rhs = tolower(rhs);

		if((lhs == '\0') && (rhs == '\0')) {
			common[ci++] = '\0';
			result = true;
			break;
		}

		if(lhs != rhs)
			break;
		else
			common[ci++] = lhs;
	}
	
	//cout << "common = " << common << endl;
	return result;
}

