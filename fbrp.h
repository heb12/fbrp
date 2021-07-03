#ifndef FBRP_H
#define FBRP_H

#define BOOK_LENGTH 30
#define MAX_READ 25
#define MAX_WORD 20

struct Reference {
	char book[BOOK_LENGTH];
	struct Chapter {
		int type;
		int range[2];
		int length;
	}chapter[5];
	struct Verse {
		int type;
		int range[2];
		int length;
	}verse[10];
	int chapterLength;
	int verseLength;
};

// !! Use these instead of the default functions,
// !! keeping the old ones until everything is fixed.
#define FbrpReference Reference
#define fbrp_parse parseReference

void parseReference(struct Reference *ref, char string[]);

#endif
