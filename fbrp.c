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

// Test type of a char
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
		int detect = determineType(string[c]);

		// 1: Break if we are on last char
		if ((c == length - 1 || lastType != determineType(string[c])) && c != 0) {
			// Once we are on the last character, append it
			// and use the last next few lines to null terminate the string
			// and set the length for looping.
			if (c == length - 1) {
				read[readY][readX] = string[c];
				readX++;
			}

			read[readY][readX] = '\0';
			printf("\"%s%s\n", read[readY], "\"");

			lengths[lengthsX] = readX;
			lengthsX++;
			readY++;

			// Once we did that, quit to next (break the loop)
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

	// This is used to append the next part in
	// a multiple/range. So setting it to 1 will make
	// the next part be on the
	int appendNext = 0;

	ref->numbersY = 0;
	int bookStarted = 0;
	for (int y = 0; y < readY; y++) {
		// Determine the type of the current part,
		// only checking the first line.
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

			// Check the type of the next part
			// Again, only the first character
			int nextType = 0;
			if (readY != y + 1) {
				nextType = determineType(read[y + 1][0]);
			}

			// Range/multiple logic. Range/multiples
			// are the same struct. For ranges,
			// [16, 17, 0, 0, 0] = 16-17
			// [16, 20, 11, 0, 0] = 16, 20, 11
			// Also, the type is stored in this struct.
			if (nextType == 4) {
				ref->numbers[ref->numbersY].type = nextType;
				ref->numbers[ref->numbersY].n[0] = result;
				appendNext = 1;

				// Skip the loop 1, so we can ignore the dash
				// (Assuming is 1 character)
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
