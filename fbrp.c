#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "reference.h"

int isSeperator(char input) {
	char *seperators = "_*&^%$#@!~`[]}{)(-.,/= ";
	for (size_t c = 0; c < strlen(seperators); c++) {
		if (seperators[c] == input) {
			return 1;
		}
	}
	
	return -1;
}

void parseReference(int *error, char *string, struct Reference *ref) {
	// Make a 2D array for parsing
	char read[10][5];
	int readX = 0;
	int readY = 0;
	
	// First, filter out the string using a
	// "waiting" technique.
	int waiting = 0;
	for (int c = 0; c < strlen(string); c++) {
		if (isSeperator(string[c]) == 1) {
			waiting = 1;
		} else {
			// Tell it to end when we got to
			// the end of the string
			if (c == strlen(string) - 1) {
				read[readY][readX] = string[c];
				readX++;
				waiting = 1;
			}
			
			// If we are waiting and we don't have 
			// a separator, we can go to the next part.
			if (waiting == 1) {
				read[readY][readX] = '\0';
				printf("Parsed: %s\n", read[readY]);
				
				waiting = 0;
				readX = 0;
				readY++;
			}
			
			read[readY][readX] = string[c];
			readX++;
		}
	}
	
	// Customizable logic to fit
	// the most common reference usages
	// This checks when the book starts,
	// And parses differently before and after.
	ref->numbersX = 0;
	int bookStarted = 0;
	for (int y = 0; y < readY; y++) {
		char *temp;
		int result = strtol(read[y], &temp, 10);
		
		if (bookStarted == 0) {
			if (*temp == '\0') {
				printf("%s\n","Detected int, assuming part of book");
			} else {
				printf("%s\n", "Detected char in integer, assuming book");
				bookStarted = 1;
			}
			
			strcat(ref->book, read[y]);
		} else if (bookStarted == 1) {
			printf("%s\n", "Appending number");
			
			ref->numbers[ref->numbersX] = result;
			ref->numbersX++;
		}
	}
}