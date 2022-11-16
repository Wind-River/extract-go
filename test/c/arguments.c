#include "arguments.h"
#include <linux/limits.h>
#include <unistd.h>

void free_arguments(struct arguments* a) {
	if(a != NULL) {
		if(a->pwd != NULL) free(a->pwd);
		if(a->sources != NULL) {
			for(int i = 0; i < a->len; i++) {
				free(a->sources[i]);
			}
			free(a->sources);
		}
		if(a->destination != NULL && a->destination != a->pwd) free(a->destination);

		free(a);
	}
}

struct arguments* new_arguments(int size) {

	struct arguments* ret = malloc(sizeof(struct arguments));

    ret->mode = "multi";
	ret->pwd = malloc(PATH_MAX);
	getcwd(ret->pwd, PATH_MAX);

	ret->len = 0;
	ret->sources = calloc(size, sizeof(void*));
	ret->destination = NULL;
	ret->enclosed = false;
    ret->verbose = 0;

	return ret;
}