// File: main.cpp
// Description: Main function, loads input data, processes it and 
// produces output.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>

#include "route.h"
#include "output_html.h"

using namespace std;

// Global objects
extern RouteContainer RoutesList;
extern RouteContainer MissingTripGroupsList;
ofstream ferr;
ofstream fwarn;
extern int missing_route_count;

// TODO: Sort start times.
int main(int argc, char * argv[])
{
	string stop_name;

	ferr.open("errors.txt");
	fwarn.open("warnings.txt");

	ferr<<"# Errors Report"<<endl<<endl;
	fwarn<<"# Warnings Report"<<endl<<endl;

	// Read routes file(s) from 'data' directory.
	// For PMPML there are two files because of merger between PCMT and PMT.
	read_routes_file("../data/pmpml/fixed-routes-pmt.csv");
	read_routes_file("../data/pmpml/fixed-routes-pcmt.csv");

	// Read trips file(s) from 'data' directory.
	// For PMPML trips data was collected and stored depot wise. 
	// There are 10 depots, hence 10 different files.
	read_trips_file("../data/pmpml/fixed-trips-bhakti-shakti-nigdi.csv", "Bhakti-Shakti");
	read_trips_file("../data/pmpml/fixed-trips-hadapsar.csv", "Hadapsar");
	read_trips_file("../data/pmpml/fixed-trips-katraj.csv", "Katraj");
	read_trips_file("../data/pmpml/fixed-trips-kothrud.csv", "Kothrud");
	read_trips_file("../data/pmpml/fixed-trips-marketyard.csv", "Market Yard");
	read_trips_file("../data/pmpml/fixed-trips-natawadi.csv", "Na Ta Wadi");
	read_trips_file("../data/pmpml/fixed-trips-punestation.csv", "Pune Station");
	read_trips_file("../data/pmpml/fixed-trips-sadgurunagar-bhosari.csv", "Bhosari");
	read_trips_file("../data/pmpml/fixed-trips-swargate.csv", "Swargate");
	read_trips_file("../data/pmpml/fixed-trips-tukaramnagar-pimpri.csv", "Tukaramnagar");

	cout << "Missing trip group count = " << MissingTripGroupsList.size() << endl;

	// process and check data. Report errors.
	process_data();
	check_data();

	// Generate useful output.
	print_html();

	// Clean up
	ferr.close();
	fwarn.close();
	cout<<"Program existing successfully."<<endl<<endl;

	return 0;
}
