CC=gcc
CCOPTS=-std=gnu99
BINS=protocol, std

.phony: clean all

all: $(BINS)

protocol: protocol.c 
	$(CC) $(CC_OPTS) -o $@ $^
std: std_protocol.c
	$(CC) $(CC_OPTS) -o $@ $^

clean:
	rm -rf *~ $(BINS) *.pgm