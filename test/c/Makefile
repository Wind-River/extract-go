CC=clang
CFLAGS=-L./usr/local/lib -larchive -lcrypto
# RSRC = $(wildcard src/*.rl)
SRC = $(wildcard ../../*.c) $(wildcard ./*.c)
BIN=extract_c.bin
OPTIONS = -e -v

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

$(BIN): $(SRC) $(RSRC)
	ragel $(RSRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

install:
	sudo apt-get update && sudo apt install libssl-dev

testall: clear $(BIN)
	valgrind $(BIN) $(OPTIONS) test/src/* -d test/dst/

test: clear $(BIN)
	valgrind $(BIN) $(OPTIONS) test/src/glib-2.32.3.tar.xz test/src/bla.jar test/src/backtrace.aarch64.core.bz2 -d test/dst/

fulltest: clear $(BIN)
	valgrind --leak-check=full $(BIN) test/src/glib-2.32.3.tar.xz test/src/bla.jar test/src/backtrace.aarch64.core.bz2 -d test/dst/

clean:
	rm -f target/main.bin
	rm -rf test/dst/*

clear:
	rm -rf test/dst/*