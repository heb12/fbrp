#include <string.h>
#include "reference.h"

// Struct for the reference reader
struct Read {
	char text[10];
	int length;
	int type;
};

enum TYPES {
	DEFAULT = 0,
	DIGIT = 1,
	ALPHA = 2,
	SEPERATOR = 3,
	RANGE = 4,
	MULTIPLE = 5
};

// Test for a char in a string. Ex: 'c' in "cake"
int testCharString(char test, char seperators[]) {
	for (size_t c = 0; seperators[c] != '\0'; c++) {
		if (seperators[c] == test) {
			return 1;
		}
	}

	return 0;
}

// Test type of a char
int determineType(char input) {
	if (testCharString(input, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
		return ALPHA;
	} else if (testCharString(input, "1234567890")) {
		return DIGIT;
	} else if (testCharString(input, "_ :./\n\0")) {
		return SEPERATOR;
	} else if (testCharString(input, "-")) {
		return RANGE;
	} else if (testCharString(input, ",")) {
		return MULTIPLE;
	}

	return SEPERATOR;
}

// Custom STRTOL like function
int strspt(char *string, char *result, int limit) {
	int integer = 0;
	int resultC = 0;
	for (int c = 0; string[c] != '\0'; c++) {
		int charType = determineType(string[c]);
		if (charType == DIGIT) {
			integer = integer * 10;
			integer += string[c] - '0';
		} else if (charType == ALPHA) {
			result[resultC] = string[c];
			resultC++;
		}
	}

	result[resultC] = '\0';
	return integer;
}

// A simple function to set an int to chapter or verse and
// add 1 to it's counter. A complicated use of pointers could have been used, but this
// is simpler.
void setInt(struct Reference *ref, int on, int currentlyOn, int value, int append) {
	if (currentlyOn == 1) {
		ref->chapter[ref->chapterLength].r[on] = value;
		ref->chapterLength += append;
	} else if (currentlyOn == 2) {
		ref->verse[ref->verseLength].r[on] = value;
		ref->verseLength += append;
	}
}


// Main parsing function. Ex:
// int *error;
// struct Reference ref;
// parseReference(error, "1 John 3 16-17, 20, 17-18", &ref)
struct Reference parseReference(int *error, char *string) {
	struct Reference ref;

	// 2D Array for efficient interpreting
	struct Read read[20];
	read[0].length = 0;
	int readX = 0;
	int readY = 0;

	int lastType = 0;
	int partType = 0;
	for (int c = 0; string[c] != '\0'; c++) {
		int type = determineType(string[c]);

		// Skip seperator, but set lastType
		if (type == SEPERATOR) {
			// Set type when on last char
			if (string[c + 1] == '\0') {
				read[readY].type = lastType;
				read[readY].text[readX] = '\0';
			}

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


		lastType = type;
	}

	// readY++ for the last part
	readY++;

	// for (int i = 0; i < readY; i++) {
	// 	printf("%s %d\n", read[i].text, read[i].type);
	// }
	//
	// return ref;



	// Now, start interpreting
	ref.chapterLength = 0;
	ref.verseLength = 0;
	ref.book[0] = '\0';

	int currentlyOn = 0;
	int jumping = 0;

	for (size_t p = 0; p < readY; p++) {
		// Skip nothing parts (triggered)
		// if (read[p].length == 0) {
		// 	continue;
		// }
		//printf("-%d-\n", read[p].text[0]);

		// Skip range/multiple chars
		if (read[p].type == RANGE || read[p].type == MULTIPLE) {
			continue;
		}

		// Try to parse what could be string, int, or both.
		char tryString[10];
		int tryInt = -1;
		tryInt = strspt(read[p].text, tryString, read[p].length);
		//printf("%s ", read[p].text);

		// If chapter added and not jumping, then set verse
		if (ref.chapterLength >= 1 && jumping == 0) {
			currentlyOn = 2;
		}

		// if book and str undefined and p != 0 then SET chapter
		if (currentlyOn == 0 && *tryString == '\0' && p != 0) {
			currentlyOn = 1;
		}

		// if book and str undefined and p == 0 then assume part of book (Ex: [3] John)
		if (currentlyOn == 0 && *tryString == '\0' && p == 0) {
			strcat(ref.book, read[p].text);
			continue;
		}

		// if book and str valid then assume book
		if (currentlyOn == 0 && *tryString != '\0') {
			strcat(ref.book, read[p].text);
			continue;
		}

		// check next type
		int nextType = 0;
		if (p != readY - 1) {
			nextType = read[p + 1].type;
		}

		// Handle previous set jumps for range/multiple
		if (jumping == 1) {
			setInt(&ref, 1, currentlyOn, tryInt, 1);
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
			setInt(&ref, 0, currentlyOn, tryInt, 0);

			jumping = 1;
			continue;
		} else if (nextType == MULTIPLE) {
			setInt(&ref, 0, currentlyOn, tryInt, 0);
			setInt(&ref, 1, currentlyOn, tryInt, 1);

			jumping = 2;
			continue;
		}

		// Regular non range-multiple digit appending
		if (tryInt != -1 && jumping == 0) {
			setInt(&ref, 0, currentlyOn, tryInt, 0);
			setInt(&ref, 1, currentlyOn, tryInt, 1);
		}
	}

	// Reduce 1 because it was incremented on the last part.
	// This is done for accurate measuring.
	//ref.verseLength -= 1;

	// Null terminate book
	strcat(ref.book, "\0");
	return ref;
}
