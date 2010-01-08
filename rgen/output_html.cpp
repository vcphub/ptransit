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
extern StopMap StopLookUpTable;

// + Description: HTML ouput.
// Print different types of HTML output pages.
void print_html()
{
	print_index_page();
	print_route_pages();

        /*
	StopMapIterator iter = StopLookUpTable.begin();
	for(; iter != StopLookUpTable.end(); iter++) {
		iter->second->print_trip_times();
	} */
}

/* 
 * Print PMPML timetable index page in English. TODO : Marathi.
 * Index page lists all routes and has links to route pages.
 * */
void print_index_page()
{
	ofstream fout;

	cout << "Generating index page ... ";
	file_open_write("html/en-index.html", fout);

	fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
	fout<<endl<<endl;
	fout<<"<html>"<<endl;
	print_head(fout);
	fout<<"<body>"<<endl;

	// For each route in RoutesList.
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		ostringstream ss;
		string filename;
		Route * route = (*iter);

		// construct output file name for this route.
		ss<< "en-" << route->get_route_id() <<".html";
		filename = ss.str();

		// Create a link to another page which has route details.
		fout<<"<a href=\""<< filename <<"\" target=\"_blank\">";
		fout<<"Route "<< route->short_name <<" ";
		fout<<"</a>";

                /*
		if(route->start_time_list.empty())
			fout<<"  <font color=\"#FF0000\">(Trips times not available.)</font>" << endl;
                */
		fout<<"<br />"<<endl;
	}

	fout<<"<h5> One link for route.</h5>"<<endl;
	fout<<"<h5> Note: Not all PMPML routes are displayed.</h5>"<<endl;

	fout << "</body>" << endl;
	fout << "</html>" << endl;
	fout.close();
	cout << " Done."<<endl;
}

/* 
 * Print English HTML page for each route.
 * Print basic information about the route.
 * Note: For some routes, trip count could be zero.
 * */
void print_route_pages() 
{
	cout << "Generating table for each route ...";
	// For each route in RoutesList.
	int empty_count = 0;
	RouteIterator iter = RoutesList.begin();
	for(; iter != RoutesList.end(); iter++) {

		Route * route = (*iter);

		// construct output file name (in dir html) for this route. 
		ostringstream ss;
		ss << "en-" << route->get_route_id() <<".html";
		string filename = "html/" + ss.str();

		// Open output HTML file
		ofstream fout;
		file_open_write(filename.c_str(), fout);

		fout<<"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
		fout<<endl<<endl;
		fout<<"<html>"<<endl;
		print_head(fout);
		fout<<"<body>"<<endl;

		print_disclaimer(fout);

		// Part 1: Print basic information about route.
		route->print_timetable_info(fout);

		// Part 2: Print all tripgroups.
		fout << "<table align=center border=1px bordercolor=gray cellpadding=1px cellspacing=0px>";
                fout << endl;
                fout << "<tr>" << endl;

                // Print trip group information as a table column.
                TripGroupIterator iter = route->tripgroup_list.begin();
                for(; iter != route->tripgroup_list.end(); iter++) {
                        TripGroup * tg = (*iter);
                        fout << "<th>" << endl;
                        tg->print_timetable_info(fout); 
                        fout << "</th>" << endl;
                }

                fout << "</tr>" << endl;
                fout << "</table>" << endl;

		fout<<"</body>"<<endl;
		fout<<"</html>"<<endl;
		fout.close();
	} // next route

	float pcnt = (float)empty_count/RoutesList.size() * 100.0; 
	cout<<" Done." << endl;
   	cout<<"Start times not available for " << pcnt << " % routes."<<endl;
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
