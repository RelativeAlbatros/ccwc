VERSION:="0.2"

debug:
	gcc main.c util.c -g -o ccwc_$(VERSION)

test:
	@gcc main.c util.c -g -o ccwc_$(VERSION)
	@./ccwc_$(VERSION) -w test.txt

build:
	@mkdir build
	gcc main.c util.c -o ccwc
	@mv ccwc build/

install: build
	cp build/ccwc ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/ccwc

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/ccwc

