

test: src/test.c src/libstr.c
	$(CC) src/test.c src/libstr.c -o test -g -Wall -Wextra
