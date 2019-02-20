all: whoosh

whoosh: whoosh.c
	PATH=$PATH\:/bin ; export PATH
	gcc -g -Wall -Wextra whoosh.c -o whoosh

clean:
	rm whoosh
