#include <stdio.h>
#include <stdlib.h>
#include "reference.h"
#include "fbrp.h"

int main() {
	int *error;
	struct Reference ref;
	parseReference(error, "1 John 3 16 17", &ref);
	
	printf("%s\n", ref.book);
	printf("%d %d\n", ref.numbers[0].n[0], ref.numbers[0].n[1]);
	printf("%d %d\n", ref.numbers[1].n[0], ref.numbers[1].n[1]);
}