#define BOOK_LENGTH 20

struct Reference {
	char book[BOOK_LENGTH];
	struct Chapter {
		int type;
		int r[2];
		int length;
	}chapter[5];
	struct Verse {
		int type;
		int r[2];
		int length;
	}verse[5];
	int chapterX; // Not used right now
	int verseX;
};
