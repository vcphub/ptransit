
// File: utils.cpp
// Description: Utility functions for handling time and strings.

#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cctype>
#include "utils.h"

using namespace std;

// Utility functions for time.

// Convert
string time_mins_to_hhmm(int total_mins)
{
	string output;
	ostringstream temp;

	int hrs = 0, mins = 0;

	hrs = total_mins/60;
	mins = total_mins % 60; // remainder
	
	if(hrs >= 12) {

		hrs -= 12;
		temp.width(2); temp.fill('0');
		temp << hrs << ":"; 
		temp.width(2); temp.fill('0');
		temp << mins;
		temp<<" PM";

	} else {
		temp.width(2); temp.fill('0');
		temp << hrs << ":"; 
		temp.width(2); temp.fill('0');
		temp << mins;
		temp<<" AM";
	}
	
	output = temp.str();	

	//cout << "output = " << output << endl;

	return output;
}

// Remove alphabets from the string
string remove_alpha(string s)
{
	string output;

	int length = s.length();

	for(int i = 0; i < length; i++) {
		char c = s[i];
		if(isdigit(c) || (c == '.'))
			output.push_back(c);
		// else ignore
	}

	return output;
}


// Utility Function
int time_string_to_mins(string input)
{
	int output;
	string str_hours, str_mins;
	int hours = 0, mins = 0;
	size_t prev_pos = 0, curr_pos = 0;

	// make input ready for conversion.
	input = remove_alpha(input);

	curr_pos = input.find_first_of('.', 0);
	str_hours = input.substr(prev_pos, curr_pos-prev_pos);

	curr_pos += 1;
	str_mins = input.substr(curr_pos, input.size()-curr_pos);

	hours = atoi(str_hours.c_str());
	mins = atoi(str_mins.c_str());

	//cout << "hours = "<< hours <<", mins = "<< mins << endl;
	output = hours * 60 + mins;

	return output;
}


// Utility function for string handling
// Comparison function
bool string_compare(string& s1, string& s2)
{
	bool result = false;
	const char * cs1 = s1.c_str();
	const char * cs2 = s2.c_str();
	int li = 0, ri = 0;
	int ci = 0; 
	int lhs, rhs;
	char common[256];

	while(1) {
		lhs = cs1[li++];
		rhs = cs2[ri++];
		
		while(isalnum(lhs) == 0 && (lhs != '\0')) 
			lhs = cs1[li++];

		while(isalnum(rhs) == 0 && (rhs != '\0')) 
			rhs = cs2[ri++];

		lhs = tolower(lhs);
		rhs = tolower(rhs);

		if((lhs == '\0') && (rhs == '\0')) {
			common[ci++] = '\0';
			result = true;
			break;
		}

		if(lhs != rhs)
			break;
		else
			common[ci++] = lhs;
	}
	
	//cout << "common = " << common << endl;
	return result;
}

