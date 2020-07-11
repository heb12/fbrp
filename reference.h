#define BOOK_LENGTH 20

struct Reference {
	char book[BOOK_LENGTH];
	struct Numbers {
		int type;
		int n[5];
		int length;
	}numbers[5];
	int numbersX; // Not used right now
	int numbersY;
};
