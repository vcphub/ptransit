// File: trips.cpp
// Description: File contains trips related functions.

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
extern RouteContainer RoutesList;
extern ofstream ferr;
extern ofstream fwarn;

// Check single line of trips data file.
void check_trip_tokenlist(string& filename, int linecnt, vector<string> tokenlist)
{
	// Errors
	if(tokenlist[0].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing route name."<<endl;
	if(tokenlist[1].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing bus id."<<endl;
	if(tokenlist[2].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing number of stops."<<endl;
	if(tokenlist[3].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing distance."<<endl;
	if(tokenlist[4].length() == 0)	
		ferr<<filename<<": line "<<linecnt<<" missing estimated time."<<endl;
}
	
// Description: Read trips data file in CSV format.
// “route-number”, “bus-id”, “no-of-stops”, “distance”, “estimated-time”, “start-times”
// Arguments: trip data file name
// Return value: void
int missing_route_count = 0;
void read_trips_file(string filename)
{
	ifstream fin(filename.c_str());
	int linecnt = 0;
	bool cflag = false;
	
	cout<<"Reading trips data file "<<filename<<" ... "<<endl;
	string buffer;
	getline(fin, buffer);
	while(!fin.eof())
	{
		size_t prev_pos = 0, curr_pos = 0;
		vector<string> tokenlist;
		string token;
		Route * route = NULL;

		linecnt++;
		// check string
		assert(buffer.length() != 0);

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

		// trips data check
		check_trip_tokenlist(filename, linecnt, tokenlist);

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
			int last_start_time = 0, correction = 0;
			for(size_t i = 5; i < tokenlist.size(); i++) {
				int start_time = 0;

				// ignore empty token because it is useless.
				if(tokenlist[i].length() == 0)
					continue;

				start_time = time_string_to_mins(tokenlist[i]);
				if(start_time == -1)
					continue;
				assert(start_time <= 1440);

				// Handle 12 hour format, used in pune station data.
				if((correction == 0) && (last_start_time != 0) && 
					((last_start_time-start_time) > 500) && (start_time < 720)) {
					correction = 720;
					cflag = true;
				}

				if(start_time < 720)
					start_time += correction;

				if(start_time > 1440) {
					cout << "start time = " << start_time << endl;
					exit(-1);
				}

				route->start_time_list.push_back(start_time);
				last_start_time = start_time;
			}
		} else {
			// report error
			missing_route_count++;
			ferr<<filename<<": route not found, "<<tokenlist[0]<<", "<<tokenlist[1]<<endl;
		}

		// get next line
		getline(fin, buffer);
	}

	if(cflag) cout << "Correction used. " << endl;
	cout << "Total lines read = " << linecnt << endl << endl;
}


