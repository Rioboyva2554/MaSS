CC = gcc
all:
	gcc src/shell.c -o rsh
clean:
	rm -r rsh
install:
	sudo cp rsh /bin
	sudo cp doc/shell /usr/local/man/man1/rsh.1
	sudo mandb
