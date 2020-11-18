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

void parse(char *text) {
	puts(text);
	struct Reference ref = parseReference(text);
	debugPrint(&ref);

	putchar('\n');
}

int main() {
	char *ref1 = "John 3 16";
	char *ref2 = "John 3 16-20, 17, 18-19";
	parse(ref1);
	parse(ref2);

	return 0;
}
