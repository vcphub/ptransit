
#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>

class Route 
{
	private:
		std::string route_id;	// unique

	public:
		Route();

		std::string short_name;
		std::string bus_id;		// indicates UP/DOWN/Extended direction
		int stop_count;
		double distance; 		// In Kms (start to end bus stop).
		int estimated_time;		// in minutes		
		static int route_count;

		// bus stop sequence in important
		std::vector<std::string> stop_list;

		// trip start times. Minutes [0, 1439]
		std::vector<int> start_time_list;

		// Member functions.
		std::string get_route_id() { return route_id; }

};

typedef std::vector<Route*> RouteContainer;
typedef std::vector<Route*>::iterator RouteIterator;

void read_routes_file(std::string filename);
void read_trips_file(std::string filename);
void check_data();

bool string_compare(std::string& s1, std::string& s2);
void print_html();

// time conversion related functions
std::string time_mins_to_hhmm(int total_mins);
int time_string_to_mins(std::string input);


#endif
