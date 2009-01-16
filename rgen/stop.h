#ifndef STOP_H
#define STOP_H

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include "utils.h"

class Route;
class Stop 
{
	public:
		std::string stop_id;	// Unique stop id.
		std::string stop_name;		// Bus stop name 

		std::vector<Route*> route_list;	// Associated routes.
		static int stop_count;		// For generating unique keys.
		Stop(std::string stop_name);
		void print_trip_times();
};

struct StopLessThan 
{
	bool operator()(std::string lhs, std::string rhs)
	{
		return string_compare(lhs, rhs) < 0;
	}
};


typedef std::map<std::string, Stop*, StopLessThan> StopMap;
typedef std::map<std::string, Stop*, StopLessThan>::iterator StopMapIterator;

typedef std::vector<Stop*> StopContainer;
typedef std::vector<Stop*>::iterator StopIterator;
std::ostream& operator<<(std::ostream& os, Stop * ps);

#endif

