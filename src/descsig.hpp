//  descsig.hpp
// 
//  Author: Scott Carlos Carrion
//  Description: Interface file for descriptive signal handlers.
//		 descsig is POSIX.1-2017 compliant. See source code comments for usage information
//
//  Copyright (c) Scott Carlos Carrion, 2020
//
//  scott.carrion@tamu.edu


// ** Important information about correctness of this software **
//
// According to the Linux manual on sigaction (https://man7.org/linux/man-pages/man2/sigaction.2.html
// accessed 05/25/2021), there exists a bug in the Linux implementation of sigaction such that "the
// kernel does not always provide meaningful values for all of the fields of the siginfo_t that are
// relevant for that signal". Unfortunately, on GNU/Linux operating systems, or operating systems
// that utilize the Linux kernel, the reliability and correctness of this software is subject to the
// prevalence of the above-discussed bug at this time. If the kernel doesn't deliver the information,
// then descsig can't deliver anything either. The only surefire way to fix this bug is to fix it at
// the kernel level, something that the author does not see a fruitful outcome doing (not only is it
// a difficult problem to fix, but it would also have to be an alteration reviewed, accepted, and
// merged into the Linux kernel at the project level, a monumental task).



#ifndef __DESCSIG_HPP__
#define __DESCSIG_HPP__

#include <signal.h>

// descsig_install_handler() description:
//
// When invoked, installs descriptive signal handlers for up to 23 signals described as "supported
// on all implementations" in the definition of <signal.h> in POSIX.1-2017. This does not
// include those marked as obsolete or optional for being compliant with the standard. The boolean argument
// install_all dictates whether all signals will have a descriptive signal handler installed for them, or
// if it should be installed only for signals associated with errors. More details below.
//
// If install_all is false, descriptive signal handler is installed for the following signals:
// SIGABRT
// SIGALRM
// SIGBUS
// SIGFPE
// SIGILL
// SIGPIPE
// SIGSEGV
// SIGTRAP
// SIGXCPU
// SIGXFSZ
//
// If install_all is true, descriptive signal handler is installed for the following signals *as well as* those listed above:
// (these mostly relate to process termination/suspension and resumption)
//
// SIGCHLD
// SIGCONT
// SIGHUP
// SIGINT
// SIGQUIT
// SIGTERM
// SIGTSTP

int descsig_install_handler(bool install_all);

// descsig_handler() description:
//
// This function is the sigaction signal handler that is installed for the signals in descsig_install_handler()
// Signal information passed when signal is raised in the siginfo_t type argument will be printed to stderr as
// appropriate, based on the signal type (note that only fields that are guaranteed to be implemented by POSIX
// will be used)
static void descsig_handler(int signo, siginfo_t* info, void* context);

#endif
