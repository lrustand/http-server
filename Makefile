.PHONY: clean
OBJECTS = $(wildcard *.c)

default: container

server: $(OBJECTS)
	gcc -static server.c -o server

container: server www
	install -d container
	cp -r www container/.
	install -d container/bin
	install -d container/etc
	install -d container/var/log
	install -m 644 /etc/mime.types container/etc/
	for P in $$(busybox --list); do \
		ln -sf busybox container/bin/$$P; \
	done
	install -m 755 /bin/busybox container/bin
	install -m 755 server container/bin/

clean:
	rm -rf container
	rm server
