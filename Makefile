#PREFIX = /usr/local
PREFIX = ${HOME}/.local

CC = gcc
CFLAGS = -O3 -Wall -Wextra

BLOCKS = ${wildcard blocks/*.c}

all: dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

dsblocks.o: dsblocks.c blocks.h shared.h
	${CC} -o $@ -c ${CFLAGS} -Wno-missing-field-initializers -Wno-unused-parameter `pkg-config --cflags x11` $<

util.o: util.c util.h shared.h
	${CC} -o $@ -c ${CFLAGS} $<

blocks/%.o: blocks/%.c blocks/%.h util.h shared.h
	${CC} -o $@ -c ${CFLAGS} -Wno-unused-parameter $<

dsblocks: dsblocks.o util.o ${BLOCKS:c=o}
	${CC} -o $@ $^ `pkg-config --libs x11`

sigdsblocks/sigdsblocks: sigdsblocks/sigdsblocks.c
	${CC} -o $@ ${CFLAGS} $<

xgetrootname/xgetrootname: xgetrootname/xgetrootname.c
	${CC} -o $@ ${CFLAGS} `pkg-config --cflags x11` $< `pkg-config --libs x11`

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
