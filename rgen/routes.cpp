// File: routes.cpp
// Description: File contains class TripGroup member function definitions.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <map>
#include "route.h"
#include "utils.h"
#include "stop.h"

using namespace std;

// Global objects
RouteContainer RoutesList;
TripGroupContainer TripGroupsList;
extern ofstream ferr;
extern ofstream fwarn;

// vijay
StopMap StopLookUpTable;

// Static objects
int Route::route_count = 0;
int TripGroup::tripgroup_count = 0;

// class Route Constructor
Route::Route(string short_name)
{
	ostringstream ss;

	Route::route_count++;
	ss << "r" << route_count;
	this->route_id = ss.str();
        this->short_name = short_name;
}

// Add tripgroups belonging to this route.
void Route::add_tripgroup(TripGroup * tg)
{
        tripgroup_list.push_back(tg);
}

// Print basic information for route timetable.
void Route::print_timetable_info(ofstream& fout)
{
	fout<<"<center><span style='font-size: 20px; font-weight:bold; font-family:Verdana'>";
        fout <<"Route = " << short_name << endl;
	fout<<"</span></center>"<<endl;
}

// Print basic information for TripGroup timetable.
void TripGroup::print_basic_info(ofstream& fout)
{
        // Display basic trip group information.
        fout<< "<div>";
        fout<< "<b>Direction : </b>";
	fout<< get_direction();
        fout<< "<br />";

	fout<<"<b>Depot(s) : </b>";
	for(size_t i = 0; i < depot_list.size(); i++) {
		fout<< depot_list[i] <<", ";
	}
        fout<< "<br />";

        fout<<"<b>Estimated time : </b>";
	if(estimated_time == 0)
		fout << "Not Available" << endl;
	else
		fout << estimated_time << " Minutes."<<endl;
        fout<< "<br />";

        fout<< "</div>";
}

// Print stops information for TripGroup timetable.
void TripGroup::print_stops_info(ofstream& fout)
{
        // Display stop names of this trip group.
	fout << "<table align=center border=1px bordercolor=gray cellpadding=1px cellspacing=0px >" << endl;
        fout << "<caption>Bus Stops</caption>" << endl; 
	for(size_t i = 0; i < stop_list.size(); i++)  {
	        fout<<"<tr style='color:black; font-family:Verdana; font-size:12px'>"<<endl;
		fout<<"<td>"<< (i+1) << "</td>" << endl;
		fout<<"<td>"<< stop_list[i] << "</td>" << endl;
	        fout<<"</tr>"<<endl;
        }
        fout << "</table><p>" <<endl;
}

// Print stops information for TripGroup timetable.
void TripGroup::print_trips_info(ofstream& fout)
{
        // Display trip start times for this trip group.
	fout << "<table align=center border=1px bordercolor=gray cellpadding=1px cellspacing=0px >" << endl;
        fout << "<caption>Trip Start Times</caption>" << endl; 
        for(size_t trip_cnt = 0; trip_cnt < start_time_list.size(); trip_cnt++) {

                if(trip_cnt != 0 && (trip_cnt % 5 == 0))
		        fout<<"</tr>"<<endl;
                if(trip_cnt % 5 == 0)
		        fout<<"<tr align='center' style='font-family:Verdana; font-size:12px'>"<<endl;

		double time_mins = start_time_list[trip_cnt];
		fout<<"\t<td nowrap=nowrap>"<< time_mins_to_hhmm((int)time_mins) <<"</td>"<<endl;

        }
        fout << "</table><p>" <<endl;

	// Shuttle may not have trip information.
	if(start_time_list.empty()) {
	        fout<<"<h5>We are sorry, trips times for this route are not available.</h5>";
	        fout<<"<h5>We are trying our best to make it available to passengers.</h5>";
	}
}

// TripGroup Constructor
TripGroup::TripGroup(Route * route)
{
	ostringstream ss;

	TripGroup::tripgroup_count++;
	ss << "t" << tripgroup_count;
	this->tripgroup_id = ss.str();

        this->route = route;
	this->distance = 0.0; // kms
	this->estimated_time = 0; // mins
}

// Get trip group direction.
string TripGroup::get_direction()
{
	ostringstream ss;
        int stop_count = stop_list.size();
        ss << stop_list[0];
        ss << "  to  ";
        ss << stop_list[stop_count-1];

        return ss.str();
}

// Sort container start_time_list.
void TripGroup::sort_start_times()
{
	// sort start_time_list
	sort(start_time_list.begin(), start_time_list.end());
}

// Add new depot name only. 
// Note: For PMPML, some tripgroups are operated by more than one depot. 
void TripGroup::add_depot(std::string depot_name)
{
	// check depot name
	assert(depot_name.length() != 0);

	// itearate over existing list
	for(size_t i = 0; i < depot_list.size(); i++) {
		if(!string_compare(depot_list[i], depot_name)) 
			return; // do not add depot_name
	}

	// depot_name is new, add it
	depot_list.push_back(depot_name);
}

// Given stop name, find existing stop object or create new one.
// Add to stops list.
void TripGroup::add_stop(std::string stop_name)
{
	Stop * stop = NULL;
	stop = StopLookUpTable[stop_name];

	if(stop == NULL) {
		stop = new Stop(stop_name);
		StopLookUpTable[stop_name] = stop;
	}
	this->stop_list.push_back(stop);
	stop->tripgroup_list.push_back(this);
}

// Find index (sequence number) of given stop object pointer.
// Used in stop visit time calculation.
// Valid value of index [0, stop_list.size()-1].
int TripGroup::get_stop_index(Stop * pstop)
{
	// For each stop pointer in this->stop_list.
	for(size_t sindex = 0; sindex < stop_list.size(); sindex++)
	{
		if(stop_list[sindex] == pstop)
			return sindex;
	}
	assert(0);
}

/* 
 * Given short name, find matching Route object.
 * Arguments: route short name
 * Return value: Route object pointer
 */
Route * find_route(string short_name)
{
	Route * route = NULL; // desired route 

	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		if(!string_compare((*iter)->short_name, short_name)) { 
			route = *iter;
			break;
		}
	}

	return route;
}

/* 
 * Given short name and bus id, find matching tripgroup object.
 * Arguments: tripgroup short name, tripgroup bus id
 * Return value: TripGroup object pointer
 */
TripGroup * find_tripgroup(string short_name, string bus_id)
{
	TripGroup * tripgroup = NULL; // desired tripgroup

	TripGroupIterator iter = TripGroupsList.begin();
	for(; iter != TripGroupsList.end(); iter++) {
                // Access trip group's route.
                string route_short_name = (*iter)->route->short_name;
		if(!string_compare(route_short_name, short_name) && 
						!string_compare((*iter)->bus_id, bus_id))  
		{
			tripgroup = *iter;
			break;
		}
	}

	return tripgroup;
}

// Check single line of tripgroups data file.
bool check_tripgroup_tokenlist(string& filename, int linecnt, vector<string> tokenlist)
{
	bool good_flag = true; // Good to create tripgroup
	// Errors
	if(tokenlist[0].length() == 0){	
		ferr<<"Master "<<filename<<": line "<<setw(6)<<linecnt<<" missing tripgroup name."<<endl;
		good_flag = false;
	} 
	if(tokenlist[1].length() == 0) {	
		ferr<<"Master "<<filename<<": line "<<setw(6)<<linecnt<<" missing bus id."<<endl;
		good_flag = false;
	} 
	if(tokenlist[2].length() == 0) {
		ferr<<"Master "<<filename<<": line "<<setw(6)<<linecnt<<" missing bus stop name."<<endl;
		good_flag = false;
	}

	// Warnings 
	if(tokenlist[3].length() == 0)	
		fwarn<<filename<<": line "<<linecnt<<" missing stage number."<<endl;
	if(tokenlist[4].length() == 0)	
		fwarn<<filename<<": line "<<linecnt<<" missing stop sequence."<<endl;
	if(tokenlist[5].length() == 0)	
		fwarn<<filename<<": line "<<linecnt<<" missing UP direction."<<endl;
	
	return good_flag;
}

/* 
 * Count number of trip groups belonging to "Shuttle" routes.
 * For some PMPML routes, no route number is assigned, they are just 
 * called "Shuttle". Ideally unique route number should be assigned.
 *
 * Shuttle statistics gives idea of schedule data quality.
 * */
int count_shuttle_tripgroups()
{
	int count = 0;
	string s1 = "Shuttal";
	string s2 = "Shuttle";
	TripGroupIterator iter = TripGroupsList.begin();
	for(; iter != TripGroupsList.end(); iter++) {
                // Get route's short name (route number".
                string route_short_name = (*iter)->route->short_name;
		if(!string_compare(route_short_name, s1)) {
			count++;
		} else if(!string_compare(route_short_name, s2)) {
			count++;
		}
	}
	return count;
}

/* 
 * Read tripgroups data file in CSV format.
 * Format : “route-number”, “bus-id”, “stop-name”, “stage-number”, “stop-sequence”, “up-direction”
 */
void read_routes_file(string filename)
{
	ifstream fin(filename.c_str());
	int linecnt = 0;
	string buffer;
	string last_route_name = "";
	string last_bus_id = "";
	Route * route = NULL;
	TripGroup * tripgroup = NULL;

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
		bool status = check_tripgroup_tokenlist(filename, linecnt, tokenlist);
		if(status == false) {
			// input data is not good, read next line
			getline(fin, buffer);
			continue;
		}

		// Use 'tokenlist' to create and populate TripGroup object
		// Case 1 : Same route short name and bus id.
		if(!string_compare(last_route_name, tokenlist[0]) && 
			!string_compare(last_bus_id, tokenlist[1])) {

			// Add stop name to existing tripgroup object
			assert(tripgroup != NULL);
			tripgroup->add_stop(tokenlist[2]);

		} else { 
		        // Case 2 : Different route short name.
                        if(string_compare(last_route_name, tokenlist[0])) {
			        // Create new route object by passing route number.
			        route = new Route(tokenlist[0]);
			        // Add object to global array.
			        RoutesList.push_back(route);
		        }

		        // Case 3 : Same route short name, different bus id.
			// Create new tripgroup object, populate using tokenlist.
			tripgroup = new TripGroup(route);
			tripgroup->bus_id = tokenlist[1];
			tripgroup->add_stop(tokenlist[2]);

                        // TripGroup object belongs to the route.
                        route->add_tripgroup(tripgroup);

			// Add object to global array.
			TripGroupsList.push_back(tripgroup);

                        // Store previous row.
			last_route_name = route->short_name;
			last_bus_id = tripgroup->bus_id;
                }

		// get next line
		getline(fin, buffer);
	}

	cout << "Total lines read = " << linecnt << endl;
	cout << "Total routes = " << RoutesList.size() << endl;
	cout << "Total tripgroups = " << TripGroupsList.size() << endl;
	cout << "Shuttle tripgroups = " << count_shuttle_tripgroups() << endl;
	cout << "Total bus stops = " << StopLookUpTable.size() << endl << endl;
}

