#!/usr/bin/env sh

TARGET="extract_go.bin"

binary () {
    go build -o $TARGET;
}

"$@"