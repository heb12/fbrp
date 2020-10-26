#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reference.h"
#include "fbrp.h"

void debugPrint(struct Reference *ref) {
	printf("Book: %s\n", ref->book);

	for (int c = 0; c < ref->chapterLength; c++) {
		printf("Chapter: %d-%d\n", ref->chapter[c].r[0], ref->chapter[c].r[1]);
	}

	for (int c = 0; c < ref->verseLength; c++) {
		printf("Verse: %d-%d\n", ref->verse[c].r[0], ref->verse[c].r[1]);
	}
}

int main(int argc, char *argv[]) {
	char *input = "John 3 16";
	input = argv[1];

	int *error;
	struct Reference ref = parseReference(error, input);
	debugPrint(&ref);
	return 0;
}
