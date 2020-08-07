#include <stdio.h>
#include <stdlib.h>
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

int main() {
	int *error;
	struct Reference ref;

	char input[50];
	memset(input, 'a', 50);
	fgets(input, 50, stdin);

	parseReference(error, input, strlen(input), &ref);
	printf("%d\n", ref.verseLength);
	debugPrint(&ref);
}
