#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structAPI.h"
#include "simulationAPI.h"

void startSimulation (bool dFlag, bool vFlag, bool rFlag, int quantum) {
	
	information i = parseFile();
	
	if (rFlag == true) {
		roundRobin(i, dFlag, vFlag, quantum);
	}
	else {
		FCFS(i, dFlag, vFlag);
	}
	
}

void FCFS (information i, bool dFlag, bool vFlag) {
	
	int finalTime = 0;
	int totalIdle = 0;
	int counter = 0;
	int totalExit = 0;
	int currentTime = 0;
	
	printf("FCFS:\n");
	
	for (int x = 0; x < i.size; x++) {
		List * eventList = malloc(sizeof(List));
		*eventList = initializeList(printTime, dummyDelete, compareTime);
		//Process
		int globalTimer = 0;
		for (int j = 0; j < i.processList[x].size; j++) {
			globalTimer += i.processList[x].threadList[j].arrivalTime;
			thread temp = i.processList[x].threadList[j];
			
			int totalCPU = 0;
			int totalIO = 0;
			
			for (int k = 0; k < temp.size; k++) {
				totalCPU += temp.allBurst[k].cpuTime;
				totalIO += temp.allBurst[k].ioTime;
			}
			
			burst * b = malloc(sizeof(burst));
			b->cpuTime = totalCPU;
			b->ioTime = totalIO;
			
			event * e = malloc(sizeof(event));
			e->arrivalTime = globalTimer;
			e->burstEvent = *b;
			e->exitBurst = i.processList[x].threadList[j].exitTime;
			e->ioState = 0;
			e->threadNumber = i.processList[x].threadList[j].threadNumber;
			i.processList[x].threadList[j].serviceTime = totalCPU;
			i.processList[x].threadList[j].ioTime = totalIO;
			insertSorted(eventList, e);
			
			if (vFlag == true) {
				printf("At time %d: Thread %d of Process %d moves from new to ready\n", globalTimer, j+1, x+1);
			}
			
		}
		
		event * e = NULL;
		
		int cpuState = 0;
		int previousTime = 0;
		
		while ((e=getFromFront(*eventList)) != NULL) {
						
			if (previousTime <= currentTime) {
				cpuState = 0;
			}
			
			if (cpuState == 0 && currentTime >= e->arrivalTime) { //READY state
				if (e->burstEvent.cpuTime > 0 && e->ioState == 0) {
					e->arrivalTime = currentTime + e->burstEvent.cpuTime + i.threadSwitch;
					previousTime = currentTime + e->burstEvent.cpuTime;
					e->burstEvent.cpuTime = 0;
					pop(eventList);
					push(eventList, e);
					cpuState = 1;
					if (vFlag == true) {
						printf("At time %d: Thread %d of Process %d moves from ready to running\n", currentTime, e->threadNumber+1,x+1);
					}
				}
				else if (e->burstEvent.ioTime > 0 && e->ioState == 0) {
					e->arrivalTime = currentTime + e->burstEvent.ioTime;
					e->burstEvent.ioTime = 0;
					e->ioState = 1;
					pop(eventList);
					push(eventList, e);
					
					if (vFlag == true) {
						printf("At time %d: Thread %d of Process %d moves from running to blocked (IO)\n", currentTime, e->threadNumber+1, x+1);
					}
					
				}
				else if (e->exitBurst > 0) {
					currentTime += e->exitBurst;
					i.processList[x].threadList[e->threadNumber - 1].exitTime = currentTime; 
					totalExit += currentTime;
					counter++;
					e->ioState = 0;
					pop(eventList);
					
					if (vFlag == true) {
						printf("At time %d: Thread %d of Process %d moves from blocked to terminated (IO)\n", currentTime, e->threadNumber+1, x+1);
					}
				}
			}
			
			//printf("%d\n", currentTime);
			if (currentTime < previousTime) {
				if (cpuState == 0) {
					totalIdle += (previousTime - currentTime);
				}
				currentTime = previousTime;
			}
			else if (getFromFront(*eventList) != NULL && ((event*)(getFromFront(*eventList)))->arrivalTime > currentTime) {
				if (cpuState == 0) {
					totalIdle += (((event*)(getFromFront(*eventList)))->arrivalTime - currentTime);
				}
				currentTime = ((event*)(getFromFront(*eventList)))->arrivalTime;
			}
			else {
				if (cpuState == 0) {
					totalIdle ++;
				}
				currentTime++;
			}
		}
		
		finalTime += currentTime + i.processSwitch;
		eventList = NULL;	
	}
		
	printf("Total time required is : %d units\n", finalTime);
	
	printf("Average turnaround time : %lf units\n", (((double)(totalExit)) / ((double)(counter + i.size))));
	
	printf("CPU Utilization is : %d\%%\n", 100 - (int)(((double)totalIdle / (double)finalTime) * 100));
	
	if (dFlag == true) {
		for (int x = 0; x < i.size; x++) {
			for (int j = 0; j < i.processList[x].size; j++) {
				printf("Thread %d of Process %d:\n\n", j + 1, x + 1);
				
				printf("arrival time : %d\n", i.processList[x].threadList[j].arrivalTime);
				printf("service time : %d\n", i.processList[x].threadList[j].serviceTime);
				printf("I/O time : %d\n", i.processList[x].threadList[j].ioTime);
				printf("turnaround time : %d\n", i.processList[x].threadList[j].exitTime);
				printf("finish time : %d\n\n", i.processList[x].threadList[j].exitTime);
				
			}
		}
	}
	
}

void roundRobin (information i, bool dFlag, bool vFlag, int quantum) {
	
	int finalTime = 0;
	int totalIdle = 0;
	int counter = 0;
	int totalExit = 0;
	int currentTime = 0;
	
	printf("Round Robin:\n");
	
	for (int x = 0; x < i.size; x++) {
		List * eventList = malloc(sizeof(List));
		*eventList = initializeList(printTime, dummyDelete, compareTime);
		//Process
		int globalTimer = 0;
		for (int j = 0; j < i.processList[x].size; j++) {
			globalTimer += i.processList[x].threadList[j].arrivalTime;
			thread temp = i.processList[x].threadList[j];
			
			int totalCPU = 0;
			int totalIO = 0;
			
			for (int k = 0; k < temp.size; k++) {
				totalCPU += temp.allBurst[k].cpuTime;
				totalIO += temp.allBurst[k].ioTime;
			}
			
			burst * b = malloc(sizeof(burst));
			b->cpuTime = totalCPU;
			b->ioTime = totalIO;
			
			event * e = malloc(sizeof(event));
			e->arrivalTime = globalTimer;
			e->burstEvent = *b;
			e->exitBurst = i.processList[x].threadList[j].exitTime;
			e->ioState = 0;	
			e->threadNumber = i.processList[x].threadList[j].threadNumber;
			i.processList[x].threadList[j].serviceTime = totalCPU;
			i.processList[x].threadList[j].ioTime = totalIO;
			insertSorted(eventList, e);
			
			if (vFlag == true) {
				printf("At time %d: Thread %d of Process %d moves from new to ready\n", globalTimer, j+1, x+1);
			}
			
		}
		
		event * e = NULL;
		
		int previousTime = 0;
		
		int cpuState = 0;
		
		while ((e=getFromFront(*eventList)) != NULL) {
			
			if (currentTime == previousTime) {
				cpuState = 0;
			}
			
			if (currentTime >= e->arrivalTime) { //READY state
				if (e->burstEvent.cpuTime > 0) {
					event * last = getFromBack(*eventList);
					e->arrivalTime = last->arrivalTime + 1;
					if (e->burstEvent.cpuTime >= quantum) {
						e->burstEvent.cpuTime -= quantum;
						currentTime += quantum;
					}
					else {
						currentTime += e->burstEvent.cpuTime;
						e->burstEvent.cpuTime = 0;
					}
					cpuState = 1;
					pop(eventList);
					push(eventList, e);
					
					if (vFlag == true) {
						printf("At time %d: Thread %d of Process %d moves from ready to running\n", currentTime, e->threadNumber+1, x+1);
					}
					
				}
				else if (e->burstEvent.ioTime > 0) {
					cpuState = 0;
					e->arrivalTime = currentTime + e->burstEvent.ioTime;
					e->burstEvent.ioTime = 0;
					pop(eventList);
					push(eventList, e);
					
					
					if (vFlag == true) {
						printf("At time %d: Thread %d of Process %d moves from running to blocked (IO)\n", currentTime, e->threadNumber+1, x+1);
					}
				}
				else if (e->exitBurst > quantum) {
					cpuState = 1;
					currentTime += quantum;
					e->exitBurst -= quantum;
					
					event * last = getFromBack(*eventList);
					
					e->arrivalTime = last->arrivalTime + 1;
					
					pop(eventList);
					push(eventList, e);
					
				}
				else {
					cpuState = 0;
					currentTime += e->exitBurst;
					i.processList[x].threadList[e->threadNumber - 1].exitTime = currentTime;
					totalExit += currentTime;
					counter++;
					pop(eventList);
					
					if (vFlag == true) {
						printf("At time %d: Thread %d of Process %d moves from blocked to terminated (IO)\n", currentTime, e->threadNumber+1, x+1);
					}

				}
			}
			else {
				totalIdle += (e->arrivalTime - currentTime);
				currentTime = e->arrivalTime;
			}
			
			if (cpuState == 0) {
				totalIdle++;
			}
			
		}
		
		finalTime += currentTime + i.processSwitch;
		eventList = NULL;	
	}
		
	printf("Total time required is : %d units\n", finalTime);
	
	printf("Average turnaround time : %lf units\n", (((double)(totalExit)) / ((double)(counter + i.size))));
	
	printf("CPU Utilization is : %d\%%\n", 100 - (int)(((double)totalIdle / (double)finalTime) * 100));
	
	if (dFlag == true) {
		for (int x = 0; x < i.size; x++) {
			for (int j = 0; j < i.processList[x].size; j++) {
				printf("Thread %d of Process %d:\n\n", j + 1, x + 1);
				
				printf("arrival time : %d\n", i.processList[x].threadList[j].arrivalTime);
				printf("service time : %d\n", i.processList[x].threadList[j].serviceTime);
				printf("I/O time : %d\n", i.processList[x].threadList[j].ioTime);
				printf("turnaround time : %d\n", i.processList[x].threadList[j].exitTime);
				printf("finish time : %d\n\n", i.processList[x].threadList[j].exitTime);
				
			}
		}
	}
	
}

information parseFile () {
	char temp[1024];
	information i;
	
	int numberOfProcesses = 0;
	int threadSwitch = 0;
	int processSwitch = 0;
	
	fgets(temp, 1024, stdin);
	sscanf(temp, "%d %d %d", &numberOfProcesses, &threadSwitch, &processSwitch);
	
	i.size = numberOfProcesses;
	i.threadSwitch = threadSwitch;
	i.processSwitch = processSwitch;
	
	i.processList = calloc(i.size, sizeof(process));
	
	for (int x = 0; x < i.size; x++) {
		int processNumber = 0;
		int numberOfThread = 0;
		fgets(temp, 1024, stdin);
		sscanf(temp, "%d %d", &processNumber, &numberOfThread);
		i.processList[x].threadList = calloc(numberOfThread, sizeof(thread));
		i.processList[x].size = numberOfThread;
		i.processList[x].processNumber = processNumber;
		
		for (int z = 0; z < numberOfThread; z++) {
			int threadNumber = 0;
			int arrivalTime = 0;
			int numberOfCPU = 0;
			fgets(temp, 1024, stdin);
			sscanf(temp, "%d %d %d", &threadNumber, &arrivalTime, &numberOfCPU);
			
			i.processList[x].threadList[z].arrivalTime = arrivalTime;
			i.processList[x].threadList[z].size = numberOfCPU - 1;
			i.processList[x].threadList[z].threadNumber = threadNumber;
			i.processList[x].threadList[z].allBurst = calloc(numberOfCPU, sizeof(burst));
			
			for (int k = 0; k < numberOfCPU - 1; k++) {
				int cpuNumber = 0;
				int cpuTime = 0;
				int ioTime = 0;
				
				fgets(temp, 1024, stdin);
				sscanf(temp, "%d %d %d", &cpuNumber, &cpuTime, &ioTime);
				i.processList[x].threadList[z].allBurst[k].cpuTime = cpuTime;
				i.processList[x].threadList[z].allBurst[k].ioTime = ioTime;
			}
			
			int cpuNumber = 0;
			int cpuTime = 0;
			
			fgets(temp, 1024, stdin);
			sscanf(temp, "%d %d", &cpuNumber, &cpuTime);
			i.processList[x].threadList[z].exitTime = cpuTime;
		}
		
	}
	

	return i;
}

void printFile (information info) {
	
	printf("Header :\n");
	printf("Thread switch : %d\n", info.threadSwitch);
	printf("Process switch : %d\n", info.processSwitch);
	
	for (int i = 0; i < info.size; i ++) {
		printf("	Process :\n");
		printf("	Number of processes : %d\n", info.processList[i].size);
		
		for (int j = 0; j < info.processList[i].size; j++) {
			printf("		Thread : \n");
			printf("		Arrival Time : %d\n", info.processList[i].threadList[j].arrivalTime);
			printf("		Number of threads : %d\n", info.processList[i].threadList[j].size);
			
			for (int k = 0; k < info.processList[i].threadList[j].size; k ++) {
				printf("			Burst : \n");
				printf("			CPU Time : %d\n", info.processList[i].threadList[j].allBurst[k].cpuTime);
				printf("			IO Time : %d\n", info.processList[i].threadList[j].allBurst[k].ioTime);
			}
			
			printf("			Exit time : %d\n", info.processList[i].threadList[j].exitTime);
			
		}
		
	}
}

int compareTime (const void * first, const void * second) {
	
	int a1 = ((event*)(first))->arrivalTime;
	int a2 = ((event*)(second))->arrivalTime;

	if ((int)a1 > (int)a2) {
		return 1;
	}
	else if ((int)a1 < (int)a2) {
		return -1;
	}
	else {
		return 0;
	}

}

char * printTime (void * toBePrinted) {
	event * temp = toBePrinted;
	
	printf("		Event : \n");
	printf("		Arrival Time : %d\n", temp->arrivalTime);
	
	return "";
}

void dummyDelete (void * toBeDeleted) {
	
}

bool fileExist (const char * fileName) {
	
	FILE * f = NULL;
	if ((f = fopen(fileName, "r"))) {
		fclose(f);
		return true;
	}
	
	return false;
}

