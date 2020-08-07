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
<<<<<<< HEAD
	parseReference(error, "John 3 16", 9, &ref);
=======
	parseReference(error, "John 3 16, 17, 20-25", &ref);
>>>>>>> 4331015cacf5e387336b5836ad1a0698868d80d1
	debugPrint(&ref);
}
