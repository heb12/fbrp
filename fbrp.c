#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "reference.h"

/*
For a quick "types" reference:
0 - Default
1 - Digit (2)
2 - Alpha (d)
3 - Seperator (. ,)
4 - Range (-)
*/

int determineType(char input) {
	if (isdigit(input)) {
		return 1;
	} else if (isalpha(input)) {
		return 2;
	}
	
	// Test seperator
	char *seperators = "_ ./";
	for (size_t c = 0; c < strlen(seperators); c++) {
		if (seperators[c] == input) {return 3;}
	}
	
	// Test range
	char *range = "-";
	for (size_t c = 0; c < strlen(range); c++) {
		if (range[c] == input) {return 4;}
	}
	
	return 0;
}

void parseReference(int *error, char *string, struct Reference *ref) {
	int length = strlen(string);
	
	// Make a 2D array for parsing
	char read[10][10];
	int readX = 0;
	int readY = 0;
	
	// Length array for extra safety
	int lengths[10];
	int lengthsX = 0;
	
	// First, filter out the string using a
	// "waiting" technique.
	int lastType = 0;
	for (int c = 0; c < length; c++) {
		// First, detect the type.
		int detect = 0;
		if (isdigit(string[c])) {
			detect = 1;
		} else if (isalpha(string[c])) {
			detect = 2;
		}
		
		// Check if the last stored type is different
		// than new detected type, then make sure we aren't on the
		// first part, and lastly check if we are on the last char,
		// append the last if so.
		if ((c == length - 1 || lastType != detect) && c != 0) {
			if (c == length - 1) {
				read[readY][readX] = string[c];
				readX++;
			}
			
			read[readY][readX] = '\0';
			printf("\"%s%s\n", read[readY], "\"");
			
			lengths[lengthsX] = readX;
			lengthsX++;
			
			readY++;
			
			// The things below this are not needed
			// Once we are on the last char
			if (c == length - 1) {
				continue;
			}
			
			readX = 0;
		}
		
		read[readY][readX] = string[c];
		readX++;
		
		lastType = detect;
	}
	
	// Customizable logic to fit
	// the most common reference usages
	// This checks when the book starts,
	// And parses differently before and after.
	
	// To Append this part to the second
	// part of a number, Ex: set x in [0, x]
	int appendNext = 0;
	
	ref->numbersY = 0;
	int bookStarted = 0;
	for (int y = 0; y < readY; y++) {
		if (determineType(read[y][0]) == 3) {
			continue;
		}
		
		char *temp;
		int result = strtol(read[y], &temp, 10);
		
		// TODO: Clean up this logic
		if (bookStarted == 0) {
			strcat(ref->book, read[y]);
			if (*temp == '\0') {
				printf("%s\n","Detected int, assuming part of book");
			} else {
				printf("%s%s%s\n", "Detected string, assuming book is: \"", ref->book, "\"");
				bookStarted = 1;
			}
		} else if (bookStarted == 1) {		
			printf("%s%d\n", "Appending number ", result);
			
			// Range/multiple logic. Range/multiples 
			// are the same struct. For ranges,
			// [16, 17, 0, 0, 0] = 16-17
			// [16, 20, 11, 0, 0] = 16, 20, 11
			// Also, the type is stored in this
			// struct.
			
			// TODO: Move range and multiple logic
			int nextType = 0;
			if (readY != y + 1) {
				nextType = determineType(read[y + 1][0]);
			}
			
			if (nextType == 4) {
				ref->numbers[ref->numbersY].type = nextType;
				ref->numbers[ref->numbersY].n[0] = result;
				appendNext = 1;
				y++;
			} else {
				if (appendNext == 1) {
					ref->numbers[ref->numbersY].n[1] = result;
					appendNext = 0;
				} else {
					ref->numbers[ref->numbersY].n[0] = result;
					ref->numbers[ref->numbersY].n[1] = result;
					ref->numbersY++;
				}
			}
			
		}
	}
		
	printf("%s\n", "Finished parsing");
}