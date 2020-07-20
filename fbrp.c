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

enum TYPES {
    DEFAULT=0,
    DIGIT=1,
    ALPHA=2,
    SEPERATOR=3,
    RANGE=4,
    MULTIPLE=5
};

// Custom STRTOL like function
int strspt(char *string, char *result, int limit) {
    int integer = 0;
    int resultC = 0;
    for (int c = 0; c < limit; c++) {
        if (isdigit(string[c])) {
            integer = integer * 10;
            integer += string[c] - '0';
        } else {
            result[resultC] = string[c];
            resultC++;
        }
    }

    result[resultC] = '\0';
    return integer;
}

// Test type of a char
int determineType(char input) {
	if (isdigit(input)) {
		return DIGIT;
	} else if (isalpha(input)) {
		return ALPHA;
	}

	// Test seperator
	char *seperators = "_ :./";
	for (size_t c = 0; c < strlen(seperators); c++) {
		if (seperators[c] == input) {return SEPERATOR;}
	}

	// Test range
	char *range = "-";
	for (size_t c = 0; c < strlen(range); c++) {
		if (range[c] == input) {return RANGE;}
	}

	// Test multiple
	char *multiple = ",";
	for (size_t c = 0; c < strlen(multiple); c++) {
		if (multiple[c] == input) {return MULTIPLE;}
	}

	return 0;
}

// A quick function to set an int to chapter or verse and
// add 1 to it's counter.
void setInt(struct Reference *ref, int on, int currentlyOn, int value, int append) {
	if (currentlyOn == 1) {
		ref->chapter[ref->chapterX].r[on] = value;
		ref->chapterX += append;
	} else if (currentlyOn == 2) {
		ref->verse[ref->verseX].r[on] = value;
		ref->verseX += append;
	}
}


// Main parsing function. Ex:
// int *error;
// struct Reference ref;
// parseReference(error, "1 John 3 16-17, 20, 17-18", &ref)
void parseReference(int *error, char *string, struct Reference *ref) {
	int length = strlen(string);

	// 2D Array for efficient interpreting
	struct Read read[20];
	int readX = 0;
	int readY = 0;

	int lastType = 0;
	int partType = 0;
	read[readY].length = 0;
	for (int c = 0; c < length; c++) {
		int type = determineType(string[c]);

		// Skip seperator, but set lastType
		if (type == SEPERATOR) {
			lastType = type;
			continue;
		}
		
      	// Jump to next part in read
		if (type != lastType && c != 0) {
			read[readY].text[readX] = '\0';
			read[readY].type = partType;

			readY++;
			read[readY].length = 0;
			readX = 0;
			partType = 0;
		}

		partType = type;

		// Append char
		read[readY].text[readX] = string[c];
		readX++;
		read[readY].length++;

		// Break when parsing is done, and add readY
		// so that it can be used as the length for the reference array
		if (c == length - 1) {
			readY++;
			read[readY].type = 0;
			break;
		}

		lastType = type;
	}

	// Now, start interpreting
	ref->chapterX = 0;
	ref->verseX = 0;
	ref->book[0] = '\0';

	int currentlyOn = 0;
	int jumping = 0;
	for (size_t p = 0; p < readY; p++) {
		// Skip range/multiple chars
		if (read[p].type == RANGE || read[p].type == MULTIPLE) {
			continue;
		}

		// Try to parse what could be string, int, or both.
		char tryString[10];
		int tryInt = -1;
		tryInt = strspt(read[p].text, tryString, read[p].length);

		// If chapter added and not jumping, then set verse
		if (ref->chapterX >= 1 && jumping == 0) {
			currentlyOn = 2;
		}

		// if book and str undefined and p != 0 then SET chapter
		if (currentlyOn == 0 && *tryString == '\0' && p != 0) {
			currentlyOn = 1;
		}

		// if book and str undefined and p == 0 then assume part of book (Ex: [3] John)
		if (currentlyOn == 0 && *tryString == '\0' && p == 0) {
			strcat(ref->book, read[p].text);
			continue;
		}

		// if book and str valid then assume book
		if (currentlyOn == 0 && *tryString != '\0') {
			strcat(ref->book, read[p].text);
			continue;
		}

		// check next type
		int nextType = 0;
		if (p != readY - 1) {
			nextType = read[p + 1].type;
		}

		// Handle previous set jumps for range/multiple
		if (jumping == 1) {
			setInt(ref, 1, currentlyOn, tryInt, 1);
			jumping = 0;

			// Multiples after range
			if (nextType == MULTIPLE) {
				jumping = 2;
				continue;
			} else {
				currentlyOn = 2;
				continue;
			}
		} else if (jumping == 2) {
			jumping = 0;
		}

		// Check for the next type (range, multiple)
		if (nextType == RANGE) {
			setInt(ref, 0, currentlyOn, tryInt, 0);

			jumping = 1;
			continue;
		} else if (nextType == MULTIPLE) {
			setInt(ref, 0, currentlyOn, tryInt, 0);
			setInt(ref, 1, currentlyOn, tryInt, 1);

			jumping = 2;
			continue;
		}

		// Regular non range-multiple digit appending
		if (tryInt != -1 && jumping == 0) {
			setInt(ref, 0, currentlyOn, tryInt, 0);
			setInt(ref, 1, currentlyOn, tryInt, 1);
		}
	}

	// Null terminate book
	strcat(ref->book, "\0");
}
