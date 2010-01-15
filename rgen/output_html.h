#ifndef OUTPUT_HTML_H
#define OUTPUT_HTML_H

#include <fstream>
#include "route.h"

// HTML output main functions
void print_html();
void print_index_page();

// Normal output
void print_route_pages(); 

// Common functions.
void print_top_navbar(std::ofstream& fout);
void print_disclaimer(std::ofstream& fout);
void print_head(std::ofstream& fout);

#endif


