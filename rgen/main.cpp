
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>

#include "route.h"

using namespace std;

extern RouteContainer RoutesList;
ofstream ferr;
ofstream fwarn;
extern int missing_route_count;

// TODO: Sort start times.
int main(int argc, char * argv[])
{
	string stop_name;

	ferr.open("errors.txt");
	fwarn.open("warnings.txt");

	// first argument: routes-file
	read_routes_file("pmt-routes.csv");
	read_routes_file("pcmt-routes.csv");

	// second argument: trips-file
	read_trips_file("fixed-trips-bhakti-shakti-nigdi.csv");
	read_trips_file("fixed-trips-hadapsar.csv");
	read_trips_file("fixed-trips-katraj.csv");
	read_trips_file("fixed-trips-kothrud.csv");
	read_trips_file("fixed-trips-marketyard.csv");
	read_trips_file("fixed-trips-natawadi.csv");
	read_trips_file("fixed-trips-punestation.csv");
	read_trips_file("fixed-trips-sadgurunagar-bhosari.csv");
	read_trips_file("fixed-trips-swargate.csv");
	read_trips_file("fixed-trips-tukaramnagar-pimpri.csv");
	cout << "Missing route count = " << missing_route_count << endl;

	// Check data
	check_data();

	print_html();

	// Clean up
	ferr.close();
	fwarn.close();

	return 0;
}
