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
 * Print PMPML timetable index page in English. 
 * Index page lists all routes and has links to route pages.
 * TODO : Marathi.
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

        print_top_navbar(fout);

	fout << "<table align=center border=1px bordercolor=gray cellpadding=1px cellspacing=0px >" << endl;
        fout << "<tr>" << endl;
        fout << "<th>Route No.</th>" << endl;
        fout << "<th>Route Name (Trips Directions)</th>" << endl;
        fout << "<th>Remark</th>" << endl;
        fout << "</tr>" << endl;

	// For each route in RoutesList.
	RouteIterator route_iter = RoutesList.begin();
	for(; route_iter != RoutesList.end(); route_iter++) {

		ostringstream ss;
		string filename;
		Route * route = (*route_iter);
                bool trips_flag = false; // Assume no trips.

		// construct output file name for this route.
		ss<< "en-" << route->get_route_id() <<".html";
		filename = ss.str();

		// Print Route Number. 
		// Create a link to route timetable HTML page.
                fout << "<tr>" << endl;
                
                fout << "<td nowrap=nowrap>" << endl;
		fout<<"<a href=\""<< filename <<"\" target=\"_blank\">";
		fout<<"Route "<< route->short_name <<" ";
		fout<<"</a>";
                fout << "</td>" << endl;

                // Print route description.
                fout << "<td style='font-size:10px'>" << endl;
                TripGroupIterator tg_iter = route->tripgroup_list.begin();
                for(; tg_iter != route->tripgroup_list.end(); tg_iter++) {
                        TripGroup * tg = *(tg_iter);
                        fout<< tg->get_direction() <<", " ; 
		        if(!tg->start_time_list.empty())
                                trips_flag = true;
                }
                fout << "</td>" << endl;

                // Print trip start status.
                if(trips_flag == false) {
                        fout << "<td style='font-size:10px'>" << endl;
		        fout<<"  <font color=\"#FF0000\">(No trip start times.)</font>" << endl;
                        fout << "</td>" << endl;
                }

                fout << "</tr>" << endl;
	}

        fout << "</table>" << endl; 

        // Finally print disclaimer.
	print_disclaimer(fout);

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
	RouteIterator route_iter = RoutesList.begin();
	for(; route_iter != RoutesList.end(); route_iter++) {

		Route * route = (*route_iter);

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

		// Part 0: Top navigation bar.
                print_top_navbar(fout);
	
		// Part 1: Print basic information about route.
		route->print_timetable_info(fout);

		// Part 2: Print all tripgroups.
		fout << "<table align=center border=1px bordercolor=gray cellpadding=1px cellspacing=0px>";
                fout << endl;

                // Print basic trip group info.
                fout << "<tr>" << endl;
                TripGroupIterator tg_iter = route->tripgroup_list.begin();
                for(; tg_iter != route->tripgroup_list.end(); tg_iter++) {
                        TripGroup * tg = (*tg_iter);
                        fout << "<td valign=top>" << endl;
                        tg->print_basic_info(fout); 
                        fout << "</td>" << endl;
                }
                fout << "</tr>" << endl;

                // Print trip group stops information.
                fout << "<tr>" << endl;
                tg_iter = route->tripgroup_list.begin();
                for(; tg_iter != route->tripgroup_list.end(); tg_iter++) {
                        TripGroup * tg = (*tg_iter);
                        fout << "<td valign=top>" << endl;
                        tg->print_stops_info(fout); 
                        fout << "</td>" << endl;
                }
                fout << "</tr>" << endl;

                // Print trip group trips information.
                fout << "<tr>" << endl;
                tg_iter = route->tripgroup_list.begin();
                for(; tg_iter != route->tripgroup_list.end(); tg_iter++) {
                        TripGroup * tg = (*tg_iter);
                        fout << "<td valign=top>" << endl;
                        tg->print_trips_info(fout); 
                        fout << "</td>" << endl;
                }
                fout << "</tr>" << endl;

                fout << "</table>" << endl;

                // Part 3 : Finally print disclaimer.
		print_disclaimer(fout);

		fout<<"</body>"<<endl;
		fout<<"</html>"<<endl;
		fout.close();
	} // next route

	float pcnt = (float)empty_count/RoutesList.size() * 100.0; 
	cout<<" Done." << endl;
   	cout<<"Start times not available for " << pcnt << " % routes."<<endl;
}

// Print navigation bar at the top of each HTML page.
void print_top_navbar(ofstream& fout)
{
        fout<< "<a href=en-index.html>Index Page</a> | ";
	fout<< "<a href=\"http://www.pmpml.org\" target=\"_blank\">PMPML Official Website</a>" << endl;
        fout<< "<hr>" << endl;
}
	
// Utility function: Print disclaimer used in all route HTML pages.
void print_disclaimer(ofstream& fout)
{
	fout<<"<h6 align=center> Disclaimer: This is unofficial PMPML timetable.";
        fout<<"This timetable is generated using PMPML schedule in December 2008. \
        Latest schedule & timetable is likely to be differ. Please contact PMPML. \
        </h6>"<< endl;
        /*
	fout<< "Actual stop times depend on many uncontrollable factors like \
	bus breakdowns, traffic conditions etc. \
	We will try our best to provide you most accurate schedule. </h6>"<< endl;
        */
}

// Utility function: Print common head used in all HTML pages.
void print_head(ofstream& fout)
{
	fout<<"<head>"<<endl;
	fout<<"<title>PMPML Bus Schedule</title>"<<endl<<endl;
	fout<<"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"<<endl;
	fout<<"</head>"<<endl;
}
