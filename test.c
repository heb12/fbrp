#include <stdio.h>
#include <stdlib.h>
#include "reference.h"
#include "fbrp.h"

int main() {
	int *error;
	struct Reference ref;
	parseReference(error, "1 John 3 v16", &ref);
}