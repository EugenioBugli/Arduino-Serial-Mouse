CC=gcc
CCOPTS=-std=gnu99
BINS=protocol

.phony: clean all

all: $(BINS)

protocol: protocol.c 
	$(CC) $(CC_OPTS) -o $@ $^

clean:
	rm -rf *~ $(BINS) *.pgm