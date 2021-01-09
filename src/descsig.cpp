//  descsig.cpp
// 
//  Author: Scott Carlos Carrion
//  Description: Implementation file for descriptive signal handlers
//
//  Copyright (c) Scott Carlos Carrion, 2020
//
//  scott.carrion@tamu.edu

#include <signal.h>
#include <errno.h>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include "descsig.hpp"

int descsig_install_handler(bool install_all)
{
	// Allocating array of sigaction struct instances on stack
	struct sigaction acts[17];

	// These handlers are all going to display more information and require
	// no other special flags. They will also mask no other signals, and use
	// a the descsig signal handler function
	for (int i = 0; i < 17; i++) { 
		acts[i].sa_flags = SA_SIGINFO;
		acts[i].sa_sigaction = descsig_handler;
		sigemptyset(&acts[i].sa_mask); 
	}

	// Installing the descsig signal handler for 10 error-related signals guaranteed to be implemented by POSIX.1-2017
	if (sigaction(SIGABRT, &acts[0], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGABRT"); return -1; };
	if (sigaction(SIGALRM, &acts[1], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGALRM"); return -1; };
	if (sigaction(SIGBUS, &acts[2], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGBUS"); return -1; };
	if (sigaction(SIGFPE, &acts[3], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGFPE"); return -1; };
	if (sigaction(SIGILL, &acts[4], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGILL"); return -1; };
	if (sigaction(SIGPIPE, &acts[5], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGPIPE"); return -1; };
	if (sigaction(SIGSEGV, &acts[6], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGSEGV"); return -1; };
	if (sigaction(SIGTRAP, &acts[7], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGTRAP"); return -1; };
	if (sigaction(SIGXCPU, &acts[8], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGXCPU"); return -1; };
	if (sigaction(SIGXFSZ, &acts[9], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGXFSZ"); return -1; };

	// If install_all is true, install signal handler for the 7 additional signals related to process termination (also guaranteed to be implemented by POSIX.1-2017)
	if (install_all) {
		if (sigaction(SIGCHLD, &acts[10], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGCHLD"); return -1; };
		if (sigaction(SIGCONT, &acts[11], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGCONT"); return -1; };
		if (sigaction(SIGHUP, &acts[12], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGHUP"); return -1; };
		if (sigaction(SIGINT, &acts[13], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGINT"); return -1; };
		if (sigaction(SIGQUIT, &acts[14], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGQUIT"); return -1; };
		if (sigaction(SIGTERM, &acts[15], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGTERM"); return -1; };
		if (sigaction(SIGTSTP, &acts[16], NULL) == -1) { perror("descsig_install_handler() failed to install handler for SIGTSTP"); return -1; };
	}

	return 0;
}


static void descsig_handler(int signo, siginfo_t* info, void* context)
{
	// First, print the signal number and description to stdout
	char* sigdesc = strsignal(info->si_signo);
	fprintf(stderr, "***Begin descsig report***\nReceived signal %d (%s)\n", info->si_signo, sigdesc);

	// Next, print some generic information about the signal
	fprintf(stderr, "Sent by pid: %d\n", info->si_pid);
	fprintf(stderr, "Real user id of sending process: %u\n", info->si_uid);
	fprintf(stderr, "Reported address of faulting instruction: 0x%llx\n", info->si_addr);

	// Next, determine the code associated with the signal
	char codedesc[128] = "\0";

	// Begin SIGILL codes
	if (signo == SIGILL) {
		switch (info->si_code) {
			case ILL_ILLOPC:
				strcpy(codedesc, "ILL_ILLOPC (Illegal opcode)");
				break;
			case ILL_ILLOPN:
				strcpy(codedesc, "ILL_ILLOPN (Illegal operand)");
				break;
			case ILL_ILLADR:
				strcpy(codedesc, "ILL_ILLADR (Illegal addressing mode)");
				break;
			case ILL_ILLTRP:
				strcpy(codedesc, "ILL_ILLTRP (Illegal trap)");
				break;
			case ILL_PRVOPC:
				strcpy(codedesc, "ILL_PRVOPC (Priveleged opcode)");
				break;
			case ILL_PRVREG:
				strcpy(codedesc, "ILL_PRVREG (Priveleged register)");
				break;
			case ILL_COPROC:
				strcpy(codedesc, "ILL_COPROC (Coprocessor error)");
				break;
			case ILL_BADSTK:
				strcpy(codedesc, "ILL_BADSTK (Internal stack error)");
				break;
			default:
				strcpy(codedesc, "Unknown code for SIGILL");
				break;
		}
	}

	// Begin SIGFPE codes
	else if (signo == SIGFPE) {
		switch (info->si_code) {
			case FPE_INTDIV:
				strcpy(codedesc, "FPE_INTDIV (Integer divide by 0)");
				break;
			case FPE_INTOVF:
				strcpy(codedesc, "FPE_INTOVF (Integer overflow)");
				break;
			case FPE_FLTDIV:
				strcpy(codedesc, "FPE_FLTDIV (Floating-point divide by 0)");
				break;
			case FPE_FLTOVF:
				strcpy(codedesc, "FPE_FLTOVF (Floating-point overflow)");
				break;
			case FPE_FLTUND:
				strcpy(codedesc, "FPE_FLTUND (Floating-point underflow)");
				break;
			case FPE_FLTRES:
				strcpy(codedesc, "FPE_FLTRES (Floating-point inexact result)");
				break;
			case FPE_FLTINV:
				strcpy(codedesc, "FPE_FLTINV (Invalid floating-point operation)");
				break;
			case FPE_FLTSUB:
				strcpy(codedesc, "FPE_FLTSUB (Subscript out of range)");
				break;
			default:
				strcpy(codedesc, "Unknown code for SIGFPE");
				break;
		}
	}

	else if (signo == SIGSEGV) {
		switch (info->si_code) {
			case SEGV_MAPERR:
				strcpy(codedesc, "SEGV_MAPERR (Address not mapped to object)");
				break;
			case SEGV_ACCERR:
				strcpy(codedesc, "SEGV_ACCERR (Invalid permissions for mapped object)");
				break;
			default:
				strcpy(codedesc, "Unknown code for SIGSEGV");
				break;
		}
	}

	else if (signo == SIGBUS) {
		switch (info->si_code) {
			case BUS_ADRALN:
				strcpy(codedesc, "BUS_ADRALN (Invalid address alignment)");
				break;
			case BUS_ADRERR:
				strcpy(codedesc, "BUS_ADRERR (Nonexistent physical address)");
				break;
			case BUS_OBJERR:
				strcpy(codedesc, "BUS_OBJERR (Object-specific hardware error)");
				break;
			default:
				strcpy(codedesc, "Unknown code for SIGBUS");
				break;
		}
	}

	else if (signo == SIGTRAP) {
		switch (info->si_code) {
			case TRAP_BRKPT:
				strcpy(codedesc, "TRAP_BRKPT (Process breakpoint)");
				break;
			case TRAP_TRACE:
				strcpy(codedesc, "TRAP_TRACE (Process trace trap)");
				break;
			default:
				strcpy(codedesc, "Unknown code for SIGTRAP");
				break;
		}
	}

	else if (signo == SIGCHLD) {
		switch (info->si_code) {
			case CLD_EXITED:
				strcpy(codedesc, "CLD_EXITED (Child exited)");
				break;
			case CLD_KILLED:
				strcpy(codedesc, "CLD_KILLED (Child terminated abnormally, did not dump core)");
				break;
			case CLD_DUMPED:
				strcpy(codedesc, "CLD_DUMPED (Child terminated abnormally, dumped core)");
				break;
			case CLD_TRAPPED:
				strcpy(codedesc, "CLD_TRAPPED (Traced child has trapped)");
				break;
			case CLD_STOPPED:
				strcpy(codedesc, "CLD_STOPPED (Child has stopped)");
				break;
			case CLD_CONTINUED:
				strcpy(codedesc, "CLD_CONTINUED (Stopped child has continued)");
				break;
			default:
				strcpy(codedesc, "Unknown code for SIGCHLD");
				break;
		}
	}

	// If the signal was not any of the above, check the generic signal codes
	else {
		switch (info->si_code) {
			case SI_USER:
				strcpy(codedesc, "SI_USER (Signal sent by kill())");
				break;
			case SI_QUEUE:
				strcpy(codedesc, "SI_QUEUE (Signal sent by sigqueue())");
				break;
			case SI_TIMER:
				strcpy(codedesc, "SI_TIMER (Signal generated by expiration of timer set by timer_settime())");
				break;
			case SI_ASYNCIO:
				strcpy(codedesc, "SI_ASYNCIO (Signal generated by completion of an asynchronous I/O request)");
				break;
			case SI_MESGQ:
				strcpy(codedesc, "SI_MESGQ (Signal generated by arrival of a message on an empty message queue)");
				break;
			default:
				strcpy(codedesc, "Unknown code");
				break;
		}
	}

	// Now that we know the signal code, print its description to stderr
	fprintf(stderr, "Signal code: %s", codedesc);

	// If signal signifies unrecoverable failure, kill the sending process immediately
	if (signo == SIGABRT || signo == SIGALRM || signo == SIGBUS || signo == SIGFPE || signo == SIGILL || signo == SIGPIPE || signo == SIGSEGV || signo == SIGXCPU || signo == SIGXFSZ) {
		fprintf(stderr, "\n***End descsig report***\nKilling sending process... ");
		raise(SIGKILL);  // Kill process that sent the signal
	}

	// Otherwise, prompt the user to continue
	else {
		char buf = 'Y';
		fprintf(stderr, "\n***End descsig report***\nContinue execution? [Y/n]");
		while (read(STDIN_FILENO, &buf, 1) < 0) { perror("Internal read error (descsig)"); }  // Blocking read from stdin, waiting for user answer

		// If user entered anything but yes ('Y' or 'y'), kill the process
		if (buf != 'Y' && buf != 'y') {
			fprintf(stderr, "\nKilling sending process... ");	
			raise(SIGKILL);
		}

		else { fprintf(stderr, "\n"); }
	}

}
