all: bin/safe-exctags

bin/safe-exctags: src/safe-exctags.c
	gcc -O -Wall -o $@ $<
