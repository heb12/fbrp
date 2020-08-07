#define BOOK_LENGTH 30


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
	}verse[10];
	int chapterLength;
	int verseLength;
};
