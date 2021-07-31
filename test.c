#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbrp.h"

struct FbrpReference ref;

void debugPrint(struct FbrpReference *ref) {
	printf("Book: %s\n", ref->book);

	for (int c = 0; c < ref->chapterLength; c++) {
		printf("Chapter: [%d]-[%d]\n", ref->chapter[c].range[0], ref->chapter[c].range[1]);
	}

	for (int c = 0; c < ref->verseLength; c++) {
		printf("Verse: [%d]-[%d]\n", ref->verse[c].range[0], ref->verse[c].range[1]);
	}
}

void parse(char *text) {
	puts(text);
	fbrp_parse(&ref, text);
	debugPrint(&ref);
	puts("------");
}

int main() {
	parse("1Kgs 1, 3-5:2-4, 3-6, 1-2");
	parse("John 3 16-20, 17, 18-19");
	parse("1 John 3-4 16-17-18-19,-,-,-");
	parse("2983yr92y8392r839");
	parse("8AUW8DQ98DU98SD9W87DUASDJA");
	parse("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!");
	parse("John 3 4294967312"); // == John 3 16 (uint overflow)
	parse("John 3 v16");
	return 0;
}
