#ifndef OUTPUT_HTML_H
#define OUTPUT_HTML_H

#include <fstream>
#include "route.h"

void print_html();
void print_index_page();
void print_disclaimer(std::ofstream& fout);
void print_head(std::ofstream& fout);

void print_stop_table(std::ofstream& fout, Route * route);
void print_route_pages(); 
void print_compact_route_pages(); 
void print_basic_route_info(std::ofstream& fout, Route * route);

#endif


