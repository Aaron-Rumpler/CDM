#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <dispatch/dispatch.h>
#include "cdm.h"
#include "sleep_assertion.h"

#define CDM_INTERVAL_MSEC 1000
#define CDM_LEEWAY_MSEC 100

#define EXIT_SIGNALS_COUNT 21

static const int EXIT_SIGNALS[EXIT_SIGNALS_COUNT] = {
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

static dispatch_queue_main_t main_queue;
static dispatch_source_t exit_dispatch_sources[EXIT_SIGNALS_COUNT];
static dispatch_source_t cdm_timer;
static dispatch_block_t exit_handler;

int main(void) {
	main_queue = dispatch_get_main_queue();
	
	if (!createSleepAssertion()) {
		return EXIT_FAILURE;
	}
	
	exit_handler = ^{
		dispatch_release(cdm_timer);
		
		for (size_t i = 0; i < EXIT_SIGNALS_COUNT; i++) {
			dispatch_source_t exit_dispatch_source = exit_dispatch_sources[i];
			
			dispatch_release(exit_dispatch_source);
		}
		
		enableCDM(false);
		
		exit(EXIT_SUCCESS);
	};
	
	for (size_t i = 0; i < EXIT_SIGNALS_COUNT; i++) {
		int exit_signal = EXIT_SIGNALS[i];
		
		signal(exit_signal, SIG_IGN);
		
		dispatch_source_t exit_dispatch_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_SIGNAL, exit_signal, 0, main_queue);
		dispatch_source_set_event_handler(exit_dispatch_source, exit_handler);
		
		exit_dispatch_sources[i] = exit_dispatch_source;
		
		dispatch_resume(exit_dispatch_source);
	}
	
	cdm_timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, main_queue);
	dispatch_source_set_timer(cdm_timer, DISPATCH_TIME_NOW, CDM_INTERVAL_MSEC * NSEC_PER_MSEC, CDM_LEEWAY_MSEC * NSEC_PER_MSEC);
	dispatch_source_set_event_handler(cdm_timer, ^{
		enableCDM(true);
	});
	
	dispatch_resume(cdm_timer);
	
	dispatch_main();
}
