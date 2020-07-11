#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "reference.h"

// Struct for the reference reader
struct Read {
	char text[10];
	int length;
	int type;
};

/*
For a quick "types" reference:
0 - Default
1 - Digit (2)
2 - Alpha (d)
3 - Seperator (. _)
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
	struct Read read[10];
	int readX = 0;
	int readY = 0;

	// First, filter out the string using a
	// "waiting" technique.
	int lastType = 0;
	for (int c = 0; c < length; c++) {
		// First, detect the type.
		int detect = determineType(string[c]);

		// Check if last type is different than this type.
		// Also, don't do it on first char or on seperators.
		if (detect != lastType && c != 0 && lastType != 3) {
			read[readY].text[readX] = '\0';
			//printf("\"%s%s\n", read[readY].text, "\"");

			// Set line lengths for safety
			read[readY].length = readX;
			read[readY].type = lastType;

			readY++;
			readX = 0;
		}

		// Don't add seperators when appending char
		if (detect != 3) {
			read[readY].text[readX] = string[c];
			readX++;
		}

		// You can probably see where I repeated myself.
		// Maybe I can find a solution in the future, but this
		// can do for now.
		if (c == length - 1) {
			read[readY].type = detect;
			read[readY].text[readX] = '\0';
			//printf("\"%s%s\n", read[readY].text, "\"");
		}

		lastType = detect;
	}

	// Lines to read are based on lines that were just added.
	// Add 1 to be exact with the last added one
	readY++;

	// After this step, the array now looks like:
	// "John 3 16-20" > ["John", "3", "16", "-", "17"]

	// Customizable logic to fit
	// the most common reference usages
	// This checks when the book starts,
	// And parses differently before and after.

	// This is used to append the next part in
	// a multiple/range. So setting it to 1 will make
	// the next part be on the
	int appendNext = 0;

	ref->numbersY = 0;
	ref->numbersX = 0;
	int bookEnded = 0;
	memset(ref->book, '\0', BOOK_LENGTH);
	for (int y = 0; y < readY; y++) {
		// Try to parse what could be string, int, or both.
		char *temp;
		int result = -1;
		result = strtol(read[y].text, &temp, 10);

		// TODO: Clean up this logic
		if (bookEnded == 0) {
			strcat(ref->book, read[y].text);
			if (*temp == '\0') {
				//printf("%s\n","Detected int, assuming part of book");
			} else {
				//printf("%s%s%s\n", "Detected string, assuming book is: \"", ref->book, "\"");
				bookEnded = 1;
			}
		} else if (bookEnded == 1) {
			// Type for this
			int thisType = 0;
			thisType = determineType(read[y].text[0]);

			// If the int parsing didn't work (read[y] is same as letters left out),
			// assume it is a multiword book. (Song of Solomon). Only do this when
			// the type is not a seperator.
			if (read[y].type != 4) {
				if (!strcmp(temp, read[y].text)) {
					//printf("%s\n", "Found extra non-int part, assuming part of book");

					// Put an extra space before it, just in case. (Song of Solomon)
					strcat(ref->book, " ");
					strcat(ref->book, read[y].text);
					continue;
				}

				//printf("%s%d\n", "Appending number ", result);
			}

			// Range/multiple logic. Range/multiples
			// are the same struct. For ranges,
			// [16, 17, 0, 0, 0] = 16-17
			// [16, 20, 11, 0, 0] = 16, 20, 11
			// Also, the type is stored in this struct.

			// When we are at a range symbol, back up, (ref->numbersY--)
			// and set the next part on the last Y.
			if (read[y].type == 4) {
				ref->numbersY--;
				ref->numbers[ref->numbersY].type = read[y].type;
				ref->numbersX++;
				continue;
			}

			ref->numbers[ref->numbersY].n[ref->numbersX] = result;

			// Add based on
			if (ref->numbersX != 0) {
				ref->numbersX = 0;
			} else {
				ref->numbersY++;
			}
		}
	}

	//printf("%s\n", "Finished parsing");
}
