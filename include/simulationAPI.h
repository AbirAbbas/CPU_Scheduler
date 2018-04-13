#ifndef SIMULATIONAPI_H
#define SIMULATIONAPI_H

#include "stdbool.h"
#include "structAPI.h"
#include "LinkedListAPI.h"

void startSimulation (bool dFlag, bool vFlag, bool rFlag, int quantum);

information parseFile ();

bool fileExist (const char * fileName);

void printFile (information info);

void bubbleSort (information info);

char * printTime (void * toBePrinted);

int compareTime (const void * first, const void * second);

void dummyDelete (void * toBeDeleted);

void FCFS (information i, bool dFlag, bool vFlag);

void roundRobin (information i, bool dFlag, bool vFlag, int quantum);

#endif
