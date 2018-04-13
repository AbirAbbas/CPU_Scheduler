#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "assert.h"
#include <unistd.h>
#include "structAPI.h"

stringInformation stringTokenizer(char * input) {
	stringInformation tempInfo;
	char **tokenizedString = NULL;
	int countDelim = 0;
	char * temp = NULL;
	
	temp = calloc(strlen(input) + 1, sizeof(char));
	
	strcpy(temp, input);
	
	for (char * counter = strtok(temp, " "); counter != NULL; counter = strtok(NULL, " ")) {
		countDelim++;
	}
	
	tokenizedString = calloc(countDelim + 1, sizeof(char*));
	//tokenizedString = calloc(1,(countDelim + 1));
	strcpy(temp, input);
	
	int size = countDelim;
	
	countDelim = 0;
	
	for (char * counter = strtok(temp, " "); counter != NULL; counter = strtok(NULL, " ")) {
		tokenizedString[countDelim] = calloc(strlen(counter) + 1, sizeof(char));
		//tokenizedString[countDelim] = calloc(1,sizeof(char) * strlen(counter) + 1);
		strcpy(tokenizedString[countDelim], counter);
		countDelim++;
	}
	
	free(temp);
		
	tempInfo.tokenizedString = tokenizedString;
	tempInfo.size = size;
	
	return tempInfo;
}

