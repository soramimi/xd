all:
	gcc main.c -o xd

install:
	sudo install -m 755 xd /usr/local/bin/xd

