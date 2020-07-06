#PREFIX = /usr/local
PREFIX = ${HOME}/.local

BLOCKS = ${wildcard blocks/*.c}

CC = gcc
CFLAGS = -Wall -Wextra -Wextra -O3
CFLAGSEXTRA = -Wno-unused-parameter -Wno-missing-field-initializers

all: dsblocks sigdsblocks

dsblocks: dsblocks.c blocks/common.h ${BLOCKS}
	${CC} -o $@ -lX11 ${CFLAGS} ${CFLAGSEXTRA} $<

sigdsblocks: sigdsblocks.c
	${CC} -o $@ ${CFLAGS} sigdsblocks.c

xgetrootname: xgetrootname.c
	${CC} -o $@ -lX11 ${CFLAGS} xgetrootname.c

clean:
	rm -f dsblocks sigdsblocks

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dsblocks ${DESTDIR}${PREFIX}/bin/dsblocks
	install -m 0755 sigdsblocks ${DESTDIR}${PREFIX}/bin/sigdsblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dsblocks ${DESTDIR}${PREFIX}/bin/sigdsblocks

.PHONY: all clean install uninstall
