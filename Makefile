CC = gcc
all:
	gcc src/shell.c -o mass
debug:
	gcc src/shell.c -o mass -g
clean:
	rm -r mass
install:
	sudo cp mass /bin
	sudo cp doc/shell /usr/local/man/man1/mass.1
	sudo mandb
