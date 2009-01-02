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

using namespace std;

// Global objects
extern RouteContainer RoutesList;

// Description: Print index page. 
// Index page lists all routes and has references to route pages.
void print_index_page()
{

	ofstream fout;
	int count = 0;

	fout.open("index.html");

	fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
	fout<<endl<<endl;

	fout<<"<html>"<<endl;

	fout<<"<head>"<<endl;
	fout<<"<title>PMPML Bus Schedule</title>"<<endl<<endl;
	fout<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"<<endl;
	fout<<"</head>"<<endl;

	fout<<"<body>"<<endl;

	// For each route in RoutesList.
	RouteIterator iter = RoutesList.begin();
	string last_route_name = "";
	for(; iter != RoutesList.end(); iter++) {

		ostringstream ss;
		string filename;
		Route * route = (*iter);

		int stop_count = route->stop_list.size();
		int trip_count = route->start_time_list.size();
		if(stop_count == 0 || trip_count == 0)
			continue;

		// construct output file name for this route.
		ss<< route->get_route_id() <<".html";
		filename = ss.str();

		// Create a link another page which has route details.
		if(!string_compare(last_route_name, route->short_name))
			fout<<"<br />"<<endl;

		fout<<"<a href=\""<<filename<<"\">";
		fout<<"Route "<<route->short_name<<" ";
		fout<<route->stop_list[0]<<" to "<<route->stop_list[stop_count-1];
		fout<<"</a><br />"<<endl;

		last_route_name = route->short_name;
		count++;
	}

	fout<<"<h5> Total links displayed above = "<<count<<"</h5>"<<endl;
	fout<<"<h5> Note: Not all PMPML routes are displayed.</h5>"<<endl;
	fout<<"<h5> Note: Shuttles operated by depots are not displayed.</h5>"<<endl;

	fout << "</body>" << endl;
	fout << "</html>" << endl;
	fout.close();
	cout << "Index page generated."<<endl;
}


// Description: Print HTML page for each route name and bus id pair.
// Calculate interval and use interpolation for obtaining stop times.
void print_html()
{
	int count = 0;

	print_index_page();

	// For each route in RoutesList.
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		ostringstream ss;
		string filename;
		ofstream fout;
		Route * route = (*iter);

		int stop_count = route->stop_list.size();
		int trip_count = route->start_time_list.size();
		if(stop_count == 0 || trip_count == 0)
			continue;

		// construct output file name for this route.
		ss<< route->get_route_id() <<".html";
		filename = ss.str();

		// Open output HTML file
		fout.open(filename.c_str());

		fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
		fout<<endl<<endl;

		fout<<"<html>"<<endl;

		fout<<"<head>"<<endl;
		fout<<"<title>PMPML Bus Schedule</title>"<<endl<<endl;
		fout<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"<<endl;
		fout<<"</head>"<<endl;

		fout<<"<body>"<<endl;
		fout<<"<h6> Disclaimer: About stop time accuracy. ";
		fout<< "Actual stop times depend on many uncontrollable factors like \
		bus breakdowns, traffic conditions etc. \
		We will try our best to provide you most accurate schedule. </h6>"<< endl;

		// Interpolation
		double interval = 0; // must be double for accurate calculation
		interval = (double)route->estimated_time/(double)(stop_count-1); // mins

		// Part 1: Print basic information about route.
		fout<<"<h4> Route = " << route->short_name <<"</h4>"<< endl;
		fout<<"<h4> Direction = " << route->stop_list[0]<<" --> ";
	   	fout<< route->stop_list[stop_count-1] <<"</h4>"<< endl;

		fout << "<h4> Number of stops = " << stop_count <<"</h4>"<< endl;
		fout << "<h4> Estimated time = " << route->estimated_time <<" Minutes </h4>"<< endl;

		fout << "<table border=1px bordercolor=gray cellpadding=2px cellspacing=0px >" << endl;
		// TODO: Use bus id
		
		// Part 2: Print Header
		// For each stop print stop name
		fout<<"<tr alight='center' >"<<endl;
		fout<<"<tr align='center' style='color:blue; font-family:Verdana; font-size:12px'>"<<endl;
		vector<string>::iterator stop_iter = route->stop_list.begin();
		for(; stop_iter != route->stop_list.end(); stop_iter++) 
			fout<<"\t<th>"<< (*stop_iter) << "</th>" << endl;
		fout<<"</tr>"<<endl;

		// Part 3: Print trips
		// For each trip	

		vector<int>::iterator st_iter = route->start_time_list.begin();
		for(; st_iter != route->start_time_list.end(); st_iter++) {
			
			fout<<"<tr align='center' style='font-family:Verdana; font-size:12px'>"<<endl;
			// For each stop
			vector<string>::iterator stop_iter = route->stop_list.begin();

			double time_mins = (*st_iter);
			for(; stop_iter != route->stop_list.end(); stop_iter++) {

				fout<<"\t<td nowrap=nowrap>"<< time_mins_to_hhmm((int)time_mins) <<"</td>"<<endl;

				time_mins += interval; // datatypes are double, for accuracy
			}

			fout<<"</tr>"<<endl;
		}

		fout<<endl;
		fout<<"</table>"<<endl;
		fout << "</body>" << endl;
		fout << "</html>" << endl;

		count++;
		//cout<<"Generated output file "<< filename << endl; 
	}

	cout<<"Total HTML files generated = " << count << endl;

}

