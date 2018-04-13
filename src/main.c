#include <stdio.h>
#include <string.h>
#include "structAPI.h"
#include "simulationAPI.h"
#include "stdbool.h"
#include <ctype.h>

int main (int argc, char ** argv) {
		
	bool dFlag = false;
	bool vFlag = false;
	bool rFlag = false;
	
	int quantum = 0;
	
	for (int i = 0; i < argc; i ++) {
		if (strcmp(argv[i], "-d") == 0) {
			dFlag = true;
		}
		
		if (strcmp(argv[i], "-v") == 0) {
			vFlag = true;
		}
		
		if (strcmp(argv[i], "-r") == 0) {
			rFlag = true;
			if (i + 1 < argc) {
				quantum = atoi(argv[i+1]);
			}
		}
	}
		
	startSimulation(dFlag, vFlag, rFlag, quantum);
	return 0;
}
