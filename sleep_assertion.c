#include "sleep_assertion.h"
#include <stdlib.h>
#include <stdbool.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/pwr_mgt/IOPM.h>
#include <IOKit/pwr_mgt/IOPMLib.h>

#define ASSERTION_NAME "cdm command-line tool"

bool createSleepAssertion(void) {
	IOPMAssertionID assertionID = 0;
	
	IOReturn result = IOPMAssertionCreateWithDescription(kIOPMAssertionTypePreventUserIdleSystemSleep,
	                                            CFSTR(ASSERTION_NAME), NULL,
	                                            NULL, NULL,
	                                            (CFTimeInterval) 0, kIOPMAssertionTimeoutActionRelease,
	                                            &assertionID);
	
	return result == kIOReturnSuccess;
}
