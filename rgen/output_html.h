#ifndef OUTPUT_HTML_H
#define OUTPUT_HTML_H

#include <fstream>
#include "route.h"

// HTML output main functions
void print_html();
void print_index_page();

// Normal output
void print_route_pages(); 

// Compact output
void print_compact_route_pages(); 
void print_stops_table(std::ofstream& fout, Route * route);

// Common functions.
void print_basic_route_info(std::ofstream& fout, Route * route);
void print_disclaimer(std::ofstream& fout);
void print_head(std::ofstream& fout);

void print_row_stop_names(std::ofstream& fout, Route * route);
void print_row_stop_ids(std::ofstream& fout, Route * route); 
void print_trip_rows(std::ofstream& fout, Route * route);


#endif


