// File : output_html.cpp
// Contains functions for generating HTML output.

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
#include "output_html.h"

using namespace std;

// Global objects
extern RouteContainer RoutesList;

// + Description: Print table containing stop names and numbers.
// Used for generating compact output.
void print_stop_table(ofstream& fout, Route * route)
{
	// 6 is estimated number.
	int col_cnt = route->stop_list.size()/6; 

	if(col_cnt == 0)
		col_cnt = (int)route->stop_list.size();

	fout<<"<table border=1px bordercolor=gray cellpadding=3px cellspacing=2px >"<<endl;

	for(size_t i = 0; i < route->stop_list.size(); i++) {
		if((i % col_cnt) == 0)
			fout<<"<tr align='center' style='color:blue; font-family:Verdana; font-size:12px'>"<<endl;
		fout<<"\t<th nowrap=nowrap>"<< route->stop_list[i] <<" (Stop "<<(i+1)<<")</th>"<<endl;
		if(((i+1) % col_cnt) == 0)
			fout<<"</tr>"<<endl;
	}

	fout<<"</table>"<<endl;
	fout<<"<br />"<<endl;
}

// Print HTML output.
void print_html()
{
	print_index_page();
	print_route_pages();
}

// Print route name and basic route information.
void print_basic_route_info(ofstream& fout, Route * route)
{
	int stop_count = route->stop_list.size();
	assert(stop_count > 0);

	fout<<"<center><span style='font-size: 20px; font-weight:bold; font-family:Verdana'>";
	fout<<"Route = " << route->short_name <<", All days of week <br />"<< endl;
	fout<<route->stop_list[0]<<" -- to --> ";
	fout<< route->stop_list[stop_count-1] << endl;
	fout<<"</span></center>"<<endl;

	fout<<"<h5> Depot = ";
	for(size_t i = 0; i < route->depot_list.size(); i++) {
		fout<<route->depot_list[i]<<", ";
	}
	fout<<"</h5>"<<endl;

	fout<<"<h5> Number of stops = "<<stop_count; 
	if(route->start_time_list.empty())
		fout<<", Number of trips = Not Available </h5>"<<endl;
	else 
		fout<<", Number of trips = "<< route->start_time_list.size() <<"</h5>"<<endl;

	if(route->estimated_time == 0)
		fout<<"<h5> Estimated time = Not Available </h5>"<< endl;
	else
		fout<<"<h5> Estimated time = "<< route->estimated_time <<" Minutes </h5>"<<endl;

}

// Print a route table row (bus stop ids).
void print_row_stop_ids(ofstream& fout, Route * route) 
{
	fout<<"<tr align='center' style='color:blue; font-family:Verdana; font-size:12px'>"<<endl;
	fout<<"\t<th>"<<""<<"</th>" << endl;
	for(size_t i = 1; i <= route->stop_list.size(); i++) 
		fout<<"\t<th>"<< "Stop "<<setw(2)<<(i)<< "</th>" << endl;
	fout<<"</tr>"<<endl;
}

// Print a route table row (bus stop names).
void print_row_stop_names(ofstream& fout, Route * route) 
{
	fout<<"<tr align='center' style='color:blue; font-family:Verdana; font-size:12px'>"<<endl;
	fout<<"\t<th>"<<""<<"</th>" << endl;
	for(size_t i = 0; i < route->stop_list.size(); i++) 
		fout<<"\t<th>"<< route->stop_list[i] << "</th>" << endl;
	fout<<"</tr>"<<endl;
}

// Route table: Print one row for each trip.
// Calculate interval and use interpolation for obtaining stop times.
void print_trip_rows(ofstream& fout, Route * route) 
{
	// Interpolation
	double interval = 0; // must be double for accurate calculation
	int stop_count = route->stop_list.size();
	assert(stop_count > 0);
	interval = (double)route->estimated_time/(double)(stop_count-1); // mins

	for(size_t trip_cnt = 0; trip_cnt < route->start_time_list.size(); trip_cnt++) {
			
		fout<<"<tr align='center' style='font-family:Verdana; font-size:12px'>"<<endl;
		double time_mins = route->start_time_list[trip_cnt];
		// Trip no.
		fout<<"\t<td nowrap=nowrap>"<<"Trip "<< (trip_cnt+1) <<"</td>"<<endl;

		// For a trip, print stop times.
		for(size_t stop_cnt = 0; stop_cnt < route->stop_list.size(); stop_cnt++) {
			fout<<"\t<td nowrap=nowrap>"<< time_mins_to_hhmm((int)time_mins) <<"</td>"<<endl;

			time_mins += interval; // datatypes are double, for accuracy
		}

		fout<<"</tr>"<<endl;
	}
}

// # Description: Print HTML page for each route name and bus id pair.
// Print basic information about the route.
// Note: For some routes, trip count could be zero.
void print_route_pages() 
{
	cout << "Generating table for each route ...";
	// For each route in RoutesList.
	int empty_count = 0;
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		ostringstream ss;
		string filename;
		ofstream fout;
		Route * route = (*iter);

		// construct output file name for this route.
		ss<< route->get_route_id() <<".html";
		filename = ss.str();
		// create html files in subdir 'html'
		filename = "html/" + filename; 

		// Open output HTML file
		fout.open(filename.c_str());
		if(!fout) {
			cout<<endl;
			cout<<"Error: please create sub-directory 'html'."<<endl<<endl;
			exit(-1);
		}

		fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
		fout<<endl<<endl;
		fout<<"<html>"<<endl;
		print_head(fout);
		fout<<"<body>"<<endl;

		print_disclaimer(fout);

		// Part 1: Print basic information about route.
		print_basic_route_info(fout, route);

		// Part 2: Print table header
		// For each stop print stop name
		fout << "<table border=1px bordercolor=gray cellpadding=1px cellspacing=0px >" << endl;

		print_row_stop_ids(fout, route);	
		print_row_stop_names(fout, route);

		// Part 3: Print trips
		print_trip_rows(fout, route);

		// Part 4: Print table footer
		print_row_stop_names(fout, route);
		print_row_stop_ids(fout, route);	

		fout<<endl;
		fout<<"</table>"<<endl;

		// Shuttle may not have trip information.
		if(route->start_time_list.empty()) {
			fout<<"<h5>We are sorry, trips times for this route are not available.</h5>";
			fout<<"<h5>We are trying our best to make it available to passengers.</h5>";
			empty_count++;
		}

		fout<<"</body>"<<endl;
		fout<<"</html>"<<endl;
		fout.close();

	} // next route

	float pcnt = (float)empty_count/RoutesList.size() * 100.0; 
	cout<<" Done." << endl;
   	cout<<"Start times not available for " << pcnt << " % routes."<<endl;
}

// Description: Print index page. 
// Index page lists all routes and has references to route pages.
void print_index_page()
{
	ofstream fout;

	cout << "Generating index page ... ";
	fout.open("html/index.html");
	if(!fout) {
		cout<<endl;
		cout<<"Error: please create sub-directory 'html'."<<endl<<endl;
		exit(-1);
	}

	fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
	fout<<endl<<endl;
	fout<<"<html>"<<endl;
	print_head(fout);
	fout<<"<body>"<<endl;

	// For each route in RoutesList.
	RouteIterator iter = RoutesList.begin();
	string last_route_name = "";
	for(; iter != RoutesList.end(); iter++) {

		ostringstream ss;
		string filename;
		Route * route = (*iter);

		int stop_count = route->stop_list.size();
		assert(stop_count > 0);
		/* trip count could be 0. */

		// construct output file name for this route.
		ss<< route->get_route_id() <<".html";
		filename = ss.str();

		// Create a link another page which has route details.
		if(!string_compare(last_route_name, route->short_name))
			fout<<"<br />"<<endl;

		fout<<"<a href=\""<<filename<<"\">";
		fout<<"Route "<<route->short_name<<" ";
		// print direction
		fout<<route->stop_list[0]<<" to "<<route->stop_list[stop_count-1];
		fout<<"</a>";

		if(route->start_time_list.empty())
			fout<<"  <font color=\"#FF0000\">(Trips times not available.)</font>" << endl;
		fout<<"<br />"<<endl;


		last_route_name = route->short_name;
	}

	fout<<"<h5> One link for each pair of route name and bus id.</h5>"<<endl;
	fout<<"<h5> Note: Not all PMPML routes are displayed.</h5>"<<endl;

	fout << "</body>" << endl;
	fout << "</html>" << endl;
	fout.close();
	cout << " Done."<<endl;
}


// Utility function: Print disclaimer used in all route HTML pages.
void print_disclaimer(ofstream& fout)
{
	fout<<"<h6> Disclaimer: About stop time accuracy. ";
	fout<< "Actual stop times depend on many uncontrollable factors like \
	bus breakdowns, traffic conditions etc. \
	We will try our best to provide you most accurate schedule. </h6>"<< endl;
}

// Utility function: Print common head used in all HTML pages.
void print_head(ofstream& fout)
{
	fout<<"<head>"<<endl;
	fout<<"<title>PMPML Bus Schedule</title>"<<endl<<endl;
	fout<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"<<endl;
	fout<<"</head>"<<endl;
}


