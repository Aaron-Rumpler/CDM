/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2020, William Gustafson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cdm.h"
#include <stdlib.h>
#include <stdbool.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/pwr_mgt/IOPM.h>
#include <IOKit/pwr_mgt/IOPMLib.h>

/*
 * Credit for the implementation goes mostly to Phil Dennis-Jordan (https://philjordan.eu)
 * who provided critical guidance and code examples via:
 * https://stackoverflow.com/questions/59594123/enabling-closed-display-mode-w-o-meeting-apples-requirements
 */

static IOReturn RootDomain_SetDisableClamShellSleep(io_connect_t root_domain_connection, bool disable) {
	uint32_t num_outputs = 0;
	uint32_t input_count = 1;
	uint64_t input[input_count];
	input[0] = (uint64_t) {disable ? 1 : 0};
	
	return IOConnectCallScalarMethod(root_domain_connection, kPMSetClamshellSleepState, input, input_count, NULL,
	                                 &num_outputs);
}

void enableCDM(bool enable) {
	io_connect_t connection = IO_OBJECT_NULL;
	io_service_t pmRootDomain = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("IOPMrootDomain"));
	
	IOServiceOpen(pmRootDomain, current_task(), 0, &connection);
	RootDomain_SetDisableClamShellSleep(connection, enable);
	IOServiceClose(connection);
}
