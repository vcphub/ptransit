
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

	assert(argc == 3);

	// first argument: routes-file
	read_routes_file(argv[1]);

	// second argument: trips-file
	read_trips_file(argv[2]);

	getchar();
	print_html();


	return 0;
}
