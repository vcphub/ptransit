
#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>

class Route 
{
	private:
	public:
		std::string short_name;
		std::string bus_id;
		int stop_count;
		int estimated_time;
		std::vector<std::string> stop_list;
		std::vector<int> start_time_list;

		Route() {}
};

typedef std::vector<Route*> RouteContainer;
typedef std::vector<Route*>::iterator RouteIterator;

void read_routes_file(std::string filename);
void read_trips_file(std::string filename);
bool string_compare(std::string& s1, std::string& s2);
void print_html();

// time conversion related functions
std::string time_mins_to_hhmm(int total_mins);
int time_string_to_mins(std::string input);


#endif
