#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <argp.h>
#include <stdbool.h>

// #include "extract.h"
// #include "filename.h"
#include "arguments.h"
// #include "sha1.h"
#include "../../logger.h"
#include "../../lib.h"
#include "../../filepath.h"

extern int logger_verbose;

const char *argp_program_version = "1.0.0";
const char *argp_program_bug_address = "<justin.yeo@windriver.com>";
static char doc[] = "libarchive based extraction utility";
static char args_doc[] = "[SOURCE...]";

static struct argp_option options[] = { 
	{ "enclose", 'e', 0, 0, "Enclose extracted output into a sha1-named folder under destination" },
	{ "verbose", 'v', 0, 0, "Verbose"},
	{ "destination", 'd', "DESTINATION", 0, "Destination Directory"},
	{ "file", 'f', "FILE", 0, "Single file mode File"},
	{ "name", 'n', "FILENAME", 0, "Single file mode Filename"},
    { 0 } 
};

void transitionToSingle(struct arguments* arguments) {
	if(strcmp(arguments->mode, "single") == 0) {
		return;
	}

	arguments->mode = "single";
	if(arguments->len > 0) {
		for(int i = 0; i < arguments->len; i++) {
			free(arguments->sources[i]);
		}
	}
	arguments->len = 0;
	return;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
	
	switch (key) {
		case 'e':
			arguments->enclosed = true;
			break;
		case 'v':			
			arguments->verbose += 1;
			break;
		case 'd':
			arguments->destination = malloc(PATH_MAX);
			realpath(arg, arguments->destination);
			break;
		case 'f':
			transitionToSingle(arguments);
			if(arguments->sources[0] != NULL) free(arguments->sources[0]);
			arguments->sources[0] = malloc(PATH_MAX);
			realpath(arg, arguments->sources[0]);
			if(arguments->len == 0) arguments->len = 1;
			break;
		case 'n':
			transitionToSingle(arguments);
			if(arguments->sources[1] != NULL) free(arguments->sources[1]);
			arguments->sources[1] = strdup(arg);
			arguments->len = 2;
			break;
		case ARGP_KEY_ARG:
			if(strcmp(arguments->mode, "multi") == 0) {
				arguments->sources[arguments->len] = malloc(PATH_MAX);
				realpath(arg, arguments->sources[arguments->len++]);
			}
			break;
		default: return ARGP_ERR_UNKNOWN;
	}
    // switch (key) {
    // case 'l': arguments->mode = LINE_MODE; break;
    // case 'w': arguments->mode = WORD_MODE; break;
    // case 'i': arguments->isCaseInsensitive = true; break;
    // case ARGP_KEY_ARG: return 0;
    // default: return ARGP_ERR_UNKNOWN;
    // }   
    return 0;
};

struct argp argParser = {
	options,
	parse_opt,
	args_doc,
	0,
	0,
	0
};

int main(int argc, char** argv) {
	char* sanityJoinTest = join("/home/zeo/Code/go/src/extract/test/_files/dst", "dates");
	printf("sanityJoinTest(\"/home/zeo/Code/go/src/extract/test/_files/dst\", \"dates\") = %s\n", sanityJoinTest);
	free(sanityJoinTest);

	struct arguments* arguments = new_arguments();
	argp_parse(&argParser, argc, argv, 0, 0, arguments);
	if(arguments->destination == NULL) arguments->destination = arguments->pwd;
	if(arguments->sources[0] == NULL) {
		elog("No source given\n");
		exit(1);
	}
	logger_verbose = arguments->verbose;

	if(strcmp(arguments->mode, "single") == 0) {
		char* source = arguments->sources[0];
		char* name = NULL;
		if(arguments->len > 1) {
			name = arguments->sources[1];
		}

		vlog("[single]: extractOne(%s, %s, %s, %d)\n", source, name, arguments->destination, arguments->enclosed);
		status stat = extractOne(source, name, arguments->destination, arguments->enclosed);
		int ret = stat->code;
		if(ret != 0) {
			if(stat->message != NULL) elog("message: %s\n", stat->message);
			if(stat->tag != NULL) elog("tag: %s\n", stat->tag);
		} else {
			printf("%s\n", stat->tag);
		}
		warning_array* wa = stat->warnings;
		if(wa != NULL && wa->nwarn > 0) {
			elog("{\"version\":\"%s\",\"warnings\":[", argp_program_version);
			for(int i = 0; i < wa->nwarn; i++) {
				warning* w = wa->warnings[i];
				if(i > 0)
					elog(",");
				char* filename = jsonEscape(w->file);
				char* message = jsonEscape(w->message);
				elog("{\"line\":%d,\"file\":\"%s\",\"message\":\"%s\"}", w->line, filename, message);
				free(filename);
				free(message);
			}
			elog("]}\n");
		}

		status_free(stat);
		free_arguments(arguments);
		return ret;
	} else if (strcmp(arguments->mode, "multi") == 0) {

		int errors = 0;
		for(int i = 0; i < arguments->len; i++) {
			status stat = extractOne(arguments->sources[i], NULL, arguments->destination, arguments->enclosed);
			if (stat->code != 0) {
				errors += 1;
				elog("%d\t", stat->code);
				if(stat->message != NULL) elog("%s\t", stat->message);
				if(stat->tag != NULL) elog("%s\n", stat->tag);
			}
			status_free(stat);
		}

		free_arguments(arguments);
		return errors > 0 ? 1 : 0;
	} else {

		elog("Unrecognized mode: %s\n", arguments->mode);
		free_arguments(arguments);
	}
}