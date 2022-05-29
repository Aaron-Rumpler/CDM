#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "cdm.h"
#include "sleep_assertion.h"

static const int EXIT_SIGNALS[] = {
		SIGHUP,
		SIGINT,
		SIGQUIT,
		SIGILL,
		SIGTRAP,
		SIGABRT,
		SIGEMT,
		SIGFPE,
		SIGKILL,
		SIGBUS,
		SIGSEGV,
		SIGSYS,
		SIGPIPE,
		SIGALRM,
		SIGTERM,
		SIGXCPU,
		SIGXFSZ,
		SIGVTALRM,
		SIGPROF,
		SIGUSR1,
		SIGUSR2,
};

static volatile bool keep_running = true;

static void exit_handler() {
	enableCDM(false);
}

static void signal_handler(__attribute__((unused)) int signal) {
	keep_running = false;
}

int main() {
	if (!createSleepAssertion()) {
		return EXIT_FAILURE;
	}
	
	atexit(exit_handler);
	
	struct sigaction sig_action;
	sig_action.sa_handler = signal_handler;
	sigfillset(&sig_action.sa_mask);
	sig_action.sa_flags = 0;
	
	sigset_t sig_mask;
	sigemptyset(&sig_mask);
	
	for (unsigned int i = 0; i < sizeof(EXIT_SIGNALS) / sizeof(EXIT_SIGNALS[0]); i++) {
		int signal_num = EXIT_SIGNALS[i];
		
		struct sigaction old_sig_action;
		sigaction(signal_num, NULL, &old_sig_action);
		
		if (old_sig_action.sa_handler == SIG_DFL) {
			sigaction(signal_num, &sig_action, NULL);
			sigaddset(&sig_mask, signal_num);
		}
	}
	
	sigset_t  old_sig_mask;
	sigprocmask(SIG_BLOCK, &sig_mask, &old_sig_mask);
	
	enableCDM(true);
	
	while (keep_running) {
		sigsuspend(&old_sig_mask);
	}
	
	return EXIT_SUCCESS;
}
