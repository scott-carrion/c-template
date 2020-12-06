//  flagopt.cpp
// 
//  Author: Scott Carlos Carrion
//  Description: Implementation file for flagopt (command line parsing class)
//
//  This program is copyrighted and may not be duplicated or distributed
//  without express permission by Scott Carlos Carrion
//
//  Copyright (c) Scott Carlos Carrion, 2020
//
//  scott.carrion@tamu.edu

#include "flagopt.hpp"
#include <unistd.h>
#include <cstring>

void flagopt::setoptstr()
{
	char r[4]; memset(r, 0, sizeof(r));
	switch (ap) {
		case argpolicy::NONE:
			r[0] = flag;
			break;

		case argpolicy::REQUIRED:
			r[0] = flag; r[1] = ':';
			break;
	}
	memcpy(optstr, r, sizeof(r));  // Copy our constructed string r to member optstr
}


int flagopt::verify(int* const argc, char** argv)
{
	// First, check if determine if the flag is set, and, it its argpolicy requires an argument, if there is one
	// Assign the private members set and arg accordingly
	char opt; opterr = 0;  // Setting opterr to 0 silences default behavior of getopt() when it returns '?'
	setoptstr();  // Make sure that we have an up-to-date optstr
	while ((opt = getopt(*argc, argv, getoptstr())) != -1) {
		if (opt == '?' && optopt == flag) { optind = 1; return -1; }  // Fail if there was an error, and it was caused by this flag
		// Otherwise, if opt is flag, then flag is included and well-formed
		else if (opt == flag) {
			// Reset optind and break, since this flag and argument were found in argv and we probably want to use argv again
			set = true; if (ap == argpolicy::REQUIRED) { arg = optarg; optind = 1; break; }  // Set arg, if applicable
		}
	}
	
	optind = 1;  // Reset this so we can parse argv again in the future
	return 0;  // All done! Exit success
}
