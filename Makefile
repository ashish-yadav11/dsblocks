#PREFIX = /usr/local
PREFIX = ${HOME}/.local

CC = gcc
CFLAGS1 = -O3 -Wall -Wextra -Wno-missing-field-initializers -Wno-unused-parameter
CFLAGS2 = -O3 -Wall -Wextra

BLOCKS = ${wildcard blocks/*.c}

all: dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

dsblocks.o: dsblocks.c blocks.h shared.h
util.o: util.c util.h shared.h
${BLOCKS:c=o}: $@ ${@:c=h} util.h shared.h

%.o: %.c
	${CC} -o $@ -c ${CFLAGS1} $<

dsblocks: dsblocks.o util.o ${BLOCKS:c=o}
	${CC} -o $@ -lX11 dsblocks.o util.o ${BLOCKS:c=o}

sigdsblocks/sigdsblocks: sigdsblocks/sigdsblocks.c
	${CC} -o $@ ${CFLAGS2} sigdsblocks/sigdsblocks.c

xgetrootname/xgetrootname: xgetrootname/xgetrootname.c
	${CC} -o $@ -lX11 ${CFLAGS2} xgetrootname/xgetrootname.c

clean:
	rm -f blocks/*.o *.o dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dsblocks ${DESTDIR}${PREFIX}/bin/dsblocks
	install -m 0755 sigdsblocks/sigdsblocks ${DESTDIR}${PREFIX}/bin/sigdsblocks
	install -m 0755 xgetrootname/xgetrootname ${DESTDIR}${PREFIX}/bin/xgetrootname

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dsblocks ${DESTDIR}${PREFIX}/bin/sigdsblocks ${DESTDIR}${PREFIX}/bin/xgetrootname

.PHONY: all clean install uninstall
