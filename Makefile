all: whoosh

whoosh: whoosh.c
	gcc -g -Wall -Wextra whoosh.c -o whoosh

clean:
	rm whoosh
