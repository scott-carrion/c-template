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
#include <cstring>
#include <sstream>
#include <cassert>
#include <unistd.h>

#define MAX_FLAGS 4  // Total number of possible flags that can be passed via the command line

// --- OPTARG PARSING FUNCTION -- //

// Contains info about an option a programmer might want to include
class flagopt {
	bool set;  // Was the flag set?
	char* arg;  // Flag's argument, if applicable

	public:
	char flag;  // e.g "a" for -a or "z" for "-z"
	// This class dictates the argument policy for this flag
	// NONE: The flag must take no arguments; e.g "-a <anything>" triggers malformation handling
	// REQUIRED: The flag must take an argument; e.g "-b" alone triggers malformation handling
	enum class argpolicy {NONE, REQUIRED};
	argpolicy ap;

	char opstr[4];

	// Constructors

	// Default constructor. Just set default zeros and nullptrs where needed
	flagopt()
	: flag('\0'),
	  ap(argpolicy::NONE),
	  opstr("\0\0\0"),
	  set(false),
	  arg(nullptr)
	{}

	// Parameterized constructor
	flagopt(const char _flag, const argpolicy _ap)
	: flag(_flag),
	  ap(_ap),
	  opstr("\0\0\0"),
	  set(false),
	  arg(nullptr)
	{}

	// Member funcs

	// Construct an opstring to use when calling getopt()
	void genopstr()
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

		memcpy(opstr, r, sizeof(r));
	}

	// verify() checks if provided references to argc and argv satisfy requirements to be well-formed relative to this flag
	// if it is, then funciton returns 0. else, it returns non-zero
	int verify(int* const argc, char** argv)
	{
		// First, check if determine if the flag is set, and, it its argpolicy requires an argument, if there is one
		// Assign the private members set and arg accordingly
		char opt; opterr = 0;
		genopstr();  // Make sure we have an up-to-date opstring first
		while ((opt = getopt(*argc, argv, opstr)) != -1) {
			if (opt == '?' && optopt == flag) { optind = 1; return -1; }
			// Otherwise, if opt is flag, then flag is included and well-formed
			else if (opt == flag) {
				// Reset optind and break, since this flag and argument were found in argv and we want tosearch again 
				set = true; if (ap == argpolicy::REQUIRED) { arg = optarg; optind = 1; break; }  
			}
		}

		optind = 1;  // Reset this so we can parse argv again in the future

		return 0;
	}
	
	bool is_set() { return set; }
	char* get_arg() { return arg; }

};


/* END DEFINITIONS */


int main(int argc, char** argv)
{
	// The variables I want to verify and set
	char* a_sv = nullptr;

	// Setting up command line parsing...
	flagopt* opts = new flagopt[MAX_FLAGS];
	opts[0] = flagopt('a', flagopt::argpolicy::REQUIRED);
	opts[1] = flagopt('b', flagopt::argpolicy::NONE);
	// More here...
	
	// Verify that program has well-formed invocation

	if (opts[0].verify(&argc, argv) < 0) { std::cerr << "Failed to verify for flag " << opts[0].flag << "!" << std::endl; }
	if (opts[1].verify(&argc, argv) < 0) { std::cerr << "Failed to verify for flag " << opts[1].flag << "!" << std::endl; }

	a_sv = opts[0].get_arg();

	// Main program below:

	if (a_sv == nullptr) { std::cout << "a variable is null" << std::endl; }
	else { std::cout << "a variable is " << a_sv << std::endl; }
	if (opts[1].is_set()) { std::cout << "b variable is true" << std::endl; }
	else { std::cout << "b variable is false" << std::endl; }
	

	std::cout << "All done! See you later!" << std::endl;

	delete[] opts;
	
	return 0;
}
