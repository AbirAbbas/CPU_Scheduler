#ifndef LINKEDLISTAPI_H
#define LINKEDLISTAPI_H

#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"

typedef struct {
	int cpuTime;
	int ioTime;
} burst;

typedef struct {
	int threadNumber;
	int arrivalTime;
	int size;
	int exitTime;
	int serviceTime;
	int ioTime;
	burst * allBurst;
	
} thread;

typedef struct {
	int processNumber;
	thread * threadList;
	int size;
} process;

typedef struct {
	process * processList;
	int size;
	int threadSwitch;
	int processSwitch;
	
} information;

typedef struct {
	char ** tokenizedString;
	int size;
} stringInformation;

typedef struct {
	int threadNumber;
	int arrivalTime;
	int exitBurst;
	int ioState;
	burst burstEvent;	
} event;

stringInformation stringTokenizer(char * input);

#endif
