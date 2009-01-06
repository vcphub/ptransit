
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

		std::string depot_name;	// Route belongs to a depot.
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
		void sort_start_times();

		// Member functions.
		std::string get_route_id() { return route_id; }

};

typedef std::vector<Route*> RouteContainer;
typedef std::vector<Route*>::iterator RouteIterator;

void read_routes_file(std::string filename);
void read_trips_file(std::string filename, std::string depot_name);
Route * find_route(std::string short_name, std::string bus_id);

void check_data();
void print_html();


#endif
