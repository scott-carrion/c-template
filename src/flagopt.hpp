//  flagopt.hpp
// 
//  Author: Scott Carlos Carrion
//  Description: Interface file for flagopt (command line parsing class)
//		 flagopt is POSIX.1-2017 compliant. See source code comments for usage information
//
//  Copyright (c) Scott Carlos Carrion, 2020
//
//  scott.carrion@tamu.edu

#ifndef __FLAGOPT_HPP__
#define __FLAGOPT_HPP__

class flagopt {
	bool set;	// Set by verify(). Tells whether flag is in argv. Only true if well-formed
	char* arg;	// Set by verify(). If flag has argpolicy NONE, this remains nullptr. Otherwise, it contains the flag's argument
	char optstr[4];  // String that is used to search for this flag in verify(). Set by setoptstr() and accessed by getoptstr()

	public:
	char flag;	// The character of the flag (e.g "-a" in argv should be tracked with flag = 'a')
	
	// argpolicy description:
	// NONE: The flag should have no arguments. If an argument is supplied for the flag, it is ignored.
	// REQUIRED: The flag should have an argument. If an argument is not supplied for the flag, verify() will fail
	enum class argpolicy {NONE, REQUIRED};
	argpolicy ap;


	// Constructors
	// Default constructor. Just set default zeros and nullptrs where needed
	flagopt()
	: flag('\0'),
	  ap(argpolicy::NONE),
	  optstr("\0\0\0"),
	  set(false),
	  arg(nullptr)
	{}

	// Parameterized constructor
	flagopt(const char _flag, const argpolicy _ap)
	: flag(_flag),
	  ap(_ap),
	  optstr("\0\0\0"),
	  set(false),
	  arg(nullptr)
	{}
	
	void setoptstr();
	char* getoptstr() { return optstr; }

	// Call this to parse argv, looking for flag according to the given argument policy
	// Returns 0 on success, nonzero on failure
	// Sets values of members set and arg
	int verify(int* const argc, char** argv);

	bool is_set() { return set; }
	char* get_arg() { return arg; }
};

#endif
