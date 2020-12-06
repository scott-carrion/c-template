/*

file: <FILENAME GOES HERE>

synopsis: <SHORT DESCRIPTION GOES HERE>

* * * * * * * * * * * * * * COPYRIGHT NOTICE * * * * * * * * * * * * * * * * * * * **
* This project and all source code herein is copyrighted by Scott Carlos Carrion    *
* and may not be utilized, sourced, distributed, or duplicated outside of its 	    *
* intended academic purpose at Texas A&M University without the express permission  *
* of the author.	Copyright (c) 2020					    *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

ACADEMIC INTEGRITY
------------------
I certify that I have listed all the sources that I have used to develop the solutions
and code to the submitted work. On my honor as an Aggie, I have neither given nor
received any unauthorized help on this academic work.

*/

/* BEGIN DEFINITIONS */

// --- PREPROCESSOR DIRECTIVES --- //
#include <iostream>
#include "flagopt.hpp"

// --- ARGUMENT PARSING FUNCTION ---
void parseargs(const size_t& max_flags, int* const argc, char** argv)
{
	// c-string destinations for the verified, raw arguments
	char* a_arg = nullptr;
	// More here...

	// Init flagopts for argv parsing
	flagopt* opts = new flagopt[max_flags];
	opts[0] = flagopt('a', flagopt::argpolicy::REQUIRED);  // "-a" requires argument in this example
	opts[1] = flagopt('h', flagopt::argpolicy::NONE);  // "-h" has no argument in this example, it displays a help/usage message
	// More here...

	// Verify the program has well-formed invocation
	int err = 0;
	for (int i = 0; i < max_flags; i++) {
		// If this if statement branches, print error message of your choice and exit
		if ((err = opts[i].verify(argc, argv)) < 0) { std::cerr << "verify() failed for flag -" << opts[i].flag << "! err code: " << err << std::endl; exit(1); }
	}

	// Finally, convert/cast arguments from flagopt instances to desired types. Perform simple input exception handling here
	a_arg = opts[0].get_arg();

	// If -h was set, print usage message and exit normally
	if (opts[1].is_set()) { std::cout << "Usage: Your usage message goes here! [-a <required dummy string arg>] [-h: Display this message]" << std::endl; exit(0); }

	// Do any argument conversion/casting from char* to desired type here...
	// It would be easiest to use global variables and assign the casted args to those

	//...

	delete[] opts;  // Cleaning up opts before end of control of function
}

/* END DEFINITIONS */

int main(int argc, char** argv)
{
	parseargs(2, &argc, argv);

	std::cout << "Hello, World!" << std::endl;

	return 0;
}
