
#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include <fstream>
#include "stop.h"

// Forward declaration
class Route;

// TripGroup : Similar trips of a route are grouped together.
class TripGroup 
{
	private:
		std::string tripgroup_id;	// unique

	public:
		TripGroup(Route * route);

		// TripGroup belongs to one or more depots.
		std::vector<std::string> depot_list;	

                Route * route;                  // Route object.
		std::string bus_id;		// indicates UP/DOWN/Extended direction
                int stop_count;                 // Bus stop count.
		double distance; 		// In Kms (start to end bus stop).
		int estimated_time;		// in minutes		
		double interval;
		static int tripgroup_count;

		// bus stop sequence in important
		StopContainer stop_list;

		// trip start times. Minutes [0, 1439]
		std::vector<int> start_time_list;
		void sort_start_times();

		// Member functions.
		std::string get_tripgroup_id() { return tripgroup_id; }
		void add_depot(std::string depot_name);
		void add_stop(std::string stop_name);
		int get_stop_index(Stop * pstop);
                std::string get_direction();
                // Timetable related methods.
                void print_basic_info(std::ofstream& fout);
                void print_stops_info(std::ofstream& fout);
                void print_trips_info(std::ofstream& fout);
};

typedef std::vector<TripGroup*> TripGroupContainer;
typedef std::vector<TripGroup*>::iterator TripGroupIterator;

void read_routes_file(std::string filename);
void read_trips_file(std::string filename, std::string depot_name);
TripGroup * find_tripgroup(std::string short_name, std::string bus_id);

// Route/Service
class Route 
{
	private:
		std::string route_id;	// unique

	public:
		Route(std::string short_name);
		std::string short_name;                 // Short names means route number.
                TripGroupContainer tripgroup_list;      // UP & DOWN tripgroups.
		static int route_count;                 // Use for generating unique id.

		// Member functions.
		std::string get_route_id() { return route_id; }
		void add_tripgroup(TripGroup * tg);
                void print_timetable_info(std::ofstream& fout);
};

typedef std::vector<Route*> RouteContainer;
typedef std::vector<Route*>::iterator RouteIterator;

Route * find_route(std::string short_name);

void process_data();
void check_data();

#endif
