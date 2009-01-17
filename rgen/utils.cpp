
// File: utils.cpp
// Description: Utility functions for handling time and strings.

#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cctype>
#include "utils.h"

using namespace std;

// Utility functions for handling time.

// Convert mins into 24 hour HH.MM format
string time_mins_to_hhmm(int total_mins)
{
	string output;
	ostringstream temp;

	int hrs = total_mins/60;
	int mins = total_mins % 60; // remainder
	
	temp.width(2); temp.fill('0');
	temp << hrs << ":"; 
	temp.width(2); temp.fill('0');
	temp << mins;
	
	output = temp.str();
	return output;
}

// Convert mins into 12 hour HH.MM format
string time_mins_to_twelve_hr_hhmm(int total_mins)
{
	string output;
	ostringstream temp;

	int hrs = total_mins/60;
	int mins = total_mins % 60; // remainder
	
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
	int output = -1;
	string str_hours, str_mins;
	int hours = 0, mins = 0;
	size_t prev_pos = 0, curr_pos = 0;

	// make input ready for conversion.
	input = remove_alpha(input);
	// Invalid cell value
	if(input.length() == 0)
		return output;

	curr_pos = input.find_first_of('.', 0);
	str_hours = input.substr(prev_pos, curr_pos-prev_pos);

	// minutes will be always 2 digits
	prev_pos = curr_pos + 1;
	str_mins = input.substr(prev_pos, 2);

	hours = atoi(str_hours.c_str());
	mins = atoi(str_mins.c_str());

	//cout << "hours = "<< hours <<", mins = "<< mins << endl;
	output = hours * 60 + mins;

	if(output > 1440) {
		//cout << "Invalid start time = "<<input<<" "<<output<< endl;
		output = output - 1440; // wrap around
	}

	return output;
}


// Utility function for string handling
// Comparison function
int string_compare(const string& s1, const string& s2)
{
	int result = -1;
	const char * cs1 = s1.c_str();
	const char * cs2 = s2.c_str();
	int li = 0, ri = 0;
	int ci = 0; 
	char common[256];

	while(1) {
		int lhs = cs1[li++];
		int rhs = cs2[ri++];
		
		while(isalnum(lhs) == 0 && (lhs != '\0')) 
			lhs = cs1[li++];

		while(isalnum(rhs) == 0 && (rhs != '\0')) 
			rhs = cs2[ri++];

		lhs = tolower(lhs);
		rhs = tolower(rhs);

		if((lhs == '\0') && (rhs == '\0')) {
			common[ci++] = '\0';
			result = 0;
			break;
		}

		if(lhs < rhs) {
			result = -1; // less than
			break;
		} else if(lhs > rhs) {
			result = 1; // greater than
			break;
		} else
			common[ci++] = lhs;
	}
	
	return result;
}

// Utility: Open file stream and check for errors.
void file_open_write(const char * filename, ofstream& fout)
{
	fout.open(filename);
	if(!fout) {
		cout<<endl;
		cout<<"Error: Could not open file '"<< filename <<"' in write mode.";
		cout<<endl<<endl;
		exit(-1);
	}
}
