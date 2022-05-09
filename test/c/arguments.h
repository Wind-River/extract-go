#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

struct arguments {
    char* mode;
	char* pwd;
	unsigned int len;
	char** sources;
	char* destination;
	bool enclosed;
	unsigned int verbose;
};

void free_arguments(struct arguments* a);

struct arguments* new_arguments();