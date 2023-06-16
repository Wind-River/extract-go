// Copyright (c) 2020 Wind River Systems, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//       http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software  distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
// OR CONDITIONS OF ANY KIND, either express or implied.

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <linux/limits.h>

#include "lib.h"
#include "logger.h"
#include "extract.h"
#include "filename.h"
#include "sha1.h"
#include "filepath.h"

void mustChdir(const char* path) {
    int err = chdir(path);
    if (err != 0) {
        elog("[mustChdir(%s) %s]\n", path, strerror(errno));
    }
}

status extractOne(const char* source, const char* sourceName, const char* dest, bool enclosed) {
    printf("extractOne(\"%s\", \"%s\", \"%s\", %d)\n", source, sourceName, dest, enclosed);
    char* name = NULL;
    if(sourceName == NULL) {
        filename_ptr sourceFileName = parseFilename(source);
        if(sourceFileName->basename != NULL) {
            filename_ptr baseFileName = parseFilename(sourceFileName->basename);
            if(baseFileName->ext == NULL || strcmp(baseFileName->ext, ".tar") != 0) {
                vlog("get_filename(%s) -> %s\n", sourceFileName->basename, sourceFileName->name);
            }
            filename_free(baseFileName);
        }
        filename_free(sourceFileName);
    }

    // if(dest != NULL) mustChdir(dest);
    char* destination;
    if(enclosed) {
        char* hash = sha1(source);
        destination = join(dest, hash);
        printf("extractOne encloded making \"%s\"\n", destination);
        mkdir(dest, 0755);
        // mustChdir(hash);
        free(hash);
    } else {
        destination = malloc(sizeof(char*)*strlen(dest));
        strcpy(destination, dest);
    }

    status ret = NULL;
    if(name == NULL) {
        ret = extract(source, sourceName, destination);
    } else {
        ret = extract(source, name, destination);
        free(name);
    }
    free(destination);

    ret->tag = malloc(PATH_MAX);
    ret->tag = getcwd(ret->tag, PATH_MAX);
    return ret;
}