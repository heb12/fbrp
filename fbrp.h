#ifndef __FBRP
#define __FBRP 1

#define BOOK_LENGTH 30
#define MAX_READ 25
#define MAX_WORD 20

struct Reference {
	char book[BOOK_LENGTH];
	struct Chapter {
		int type;
		int r[2];
		size_t length;
	}chapter[5];
	struct Verse {
		int type;
		int r[2];
		size_t length;
	}verse[10];
	size_t chapterLength;
	size_t verseLength;
};

void parseReference(struct Reference *ref, char *string);

#endif
