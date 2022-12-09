#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cdm.h"

int main(int argc, char **argv) {
	if (argc == 2) {
		if (strcmp(argv[1], "true") == 0) {
			enableCDM(true);
			return EXIT_SUCCESS;
		} else if (strcmp(argv[1], "false") == 0) {
			enableCDM(false);
			return EXIT_SUCCESS;
		}
	}
	
	if (argc >= 1) {
		fprintf(stderr, "Usage: %s true|false\n", argv[0]);
	}
	
	return EXIT_FAILURE;
}
