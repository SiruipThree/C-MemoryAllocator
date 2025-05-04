 # Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address

# Default rule
all: memgrind memtest

memgrind: mymalloc.c mymalloc.h memgrind.c
	$(CC) $(CFLAGS) -o memgrind mymalloc.c memgrind.c

memtest: mymalloc.c mymalloc.h memtest.c
	$(CC) $(CFLAGS) -o memtest mymalloc.c memtest.c

# Clean up build artifacts
clean:
	rm -f memgrind memtest

# Phony target for clean
.PHONY: all clean