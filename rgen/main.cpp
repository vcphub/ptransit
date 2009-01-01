
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdio>

#include "route.h"

using namespace std;

extern RouteContainer RoutesList;

int main(int argc, char * argv[])
{
	string stop_name;

	// first argument: routes-file
	read_routes_file("pmt-routes.csv");
	getchar();
	read_routes_file("pcmt-routes.csv");
	getchar();

	// second argument: trips-file
	read_trips_file("fixed-trips-hadapsar.csv");
	getchar();
	read_trips_file("fixed-trips-katraj.csv");

	// Check data
	check_data();

	print_html();

	return 0;
}
