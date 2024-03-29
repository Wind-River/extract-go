// Copyright (c) 2020 Wind River Systems, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//       http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software  distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
// OR CONDITIONS OF ANY KIND, either express or implied.

#include "status.h"

#include <stdlib.h>
#include <string.h>

status report_status(int code, const char* message, const char* tag, warning_array* wa) {
	status ret = malloc(sizeof (struct exit_struct));
	ret->code = code;
	ret->warnings = wa;

	if(message == NULL) {
		ret->message = NULL;
	} else {
		ret->message = malloc(sizeof(char)*(strlen(message)+1));
		strcpy(ret->message, message);
	}

	if(tag == NULL) {
		ret->tag = NULL;
	} else {
		ret->tag = malloc(sizeof(char)*(strlen(tag)+1));
		strcpy(ret->tag, tag);
	}

	return ret;
}

status success(warning_array* wa) {
	status ret = malloc(sizeof (struct exit_struct));
	ret->code = 0;
	ret->message = NULL;
	ret->tag = NULL;
	ret->warnings = wa;

	return ret;
}

void status_free(status stat) {
	if(stat->message != NULL) {
		free(stat->message);
	}
	if(stat->tag != NULL) {
		free(stat->tag);
	}
	if(stat->warnings != NULL) {
		warning_array_free(stat->warnings);
	}

	free(stat);
}