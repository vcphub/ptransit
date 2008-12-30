// File : output_html.cpp
// Contains functions for generating HTML output.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include "route.h"

using namespace std;

// Global objects
extern RouteContainer RoutesList;

// Description: Print HTML page for each page
// Use interpolation
void print_html()
{
	ofstream fout("output.html");

	fout<<"<html>"<<endl;
	fout<<"<title>PMPML Bus Schedule</title>"<<endl<<endl;

	fout<<"<header>"<<endl;
	fout<<"<h3> PMPML Schedule </h3>"<< endl;
	fout<<"</header>"<<endl;

	fout<<"<body>"<<endl;

	// For each route in RoutesList.
	int print_count = 0;
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		Route * route = (*iter);
		int stop_count = route->stop_list.size();
		int trip_count = route->start_time_list.size();

		double interval = 0; // must be double for accurate calculation

		// Interpolation
		if(stop_count == 0 || trip_count == 0)
			continue;

		interval = (double)route->estimated_time/(double)(stop_count-1); // mins

		// Part 1: Print basic information about route.
		fout << "<h4> Route " << route->short_name <<"</h4>"<< endl;
		fout << "<h4> Number of stops = " << stop_count <<"</h4>"<< endl;
		fout << "<h4> Estimated time = " << route->estimated_time <<" Minutes </h4>"<< endl;

		fout << "<table border=\"1\">" << endl;
		// TODO: Use bus id
		
		// Part 2: Print Header
		// For each stop print stop name
		fout<<"<tr>"<<endl;
		vector<string>::iterator stop_iter = route->stop_list.begin();
		for(; stop_iter != route->stop_list.end(); stop_iter++) 
			fout<<"\t<th>"<< (*stop_iter) << "</th>" << endl;
		fout<<"</tr>"<<endl;

		// Part 3: Print trips
		// For each trip	
		vector<int>::iterator st_iter = route->start_time_list.begin();
		for(; st_iter != route->start_time_list.end(); st_iter++) {
			
			fout<<"<tr>"<<endl;
			// For each stop
			vector<string>::iterator stop_iter = route->stop_list.begin();

			double time_mins = (*st_iter);
			for(; stop_iter != route->stop_list.end(); stop_iter++) {

				fout<<"\t<td>"<< time_mins_to_hhmm((int)time_mins) <<"</td>"<<endl;

				time_mins += interval; // datatypes are double, for accuracy
			}

			fout<<"</tr>"<<endl;
		}

		fout<<endl;
		fout<<"</table>"<<endl;
		getchar();
		print_count++;

		break;
	}

	fout << "</body>" << endl;
	fout << "</html>" << endl;
}

