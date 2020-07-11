#include <stdio.h>
#include <stdlib.h>
#include "reference.h"
#include "fbrp.h"

void debugPrint(struct Reference ref) {

}

int main() {
	int *error;
	struct Reference ref;
	parseReference(error, "John 3 5-6", &ref);

	/*
	For a quick recap of how the parsed
	verse structure is stored:
	"1John"

	Digit
	3, 0

	Range
	16, 17
	*/

	printf("%s\n", ref.book);
	printf("%d %d\n", ref.numbers[0].n[0], ref.numbers[0].n[1]);
	printf("%d %d\n", ref.numbers[1].n[0], ref.numbers[1].n[1]);
}
