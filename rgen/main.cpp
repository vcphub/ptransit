
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

int main(int argc, char * argv[])
{
	string stop_name;

	ferr.open("errors.txt");
	fwarn.open("warnings.txt");

	// first argument: routes-file
	read_routes_file("pmt-routes.csv");
	read_routes_file("pcmt-routes.csv");

	// second argument: trips-file
	read_trips_file("fixed-trips-hadapsar.csv");
	read_trips_file("fixed-trips-katraj.csv");

	// Check data
	check_data();

	print_html();

	// Clean up
	ferr.close();
	fwarn.close();

	return 0;
}
