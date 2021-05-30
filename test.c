#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbrp.h"

struct Reference ref;

void debugPrint(struct Reference *ref) {
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
	parseReference(&ref, text);
	debugPrint(&ref);
	puts("------");
}

int main() {
	parse("ABCDEFGHIJKLMNOPQSRTYCUVZ 3 16");
	parse("John 3 16-20, 17, 18-19");
	parse("1 John 3-4 16-17-18-19,-,-,-");
	parse("2983yr92y8392r839");
	parse("8AUW8DQ98DU98SD9W87DUASDJA");
	parse("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!");
	parse("John 3 4294967312"); // == John 3 16 (uint overflow)
	return 0;
}
