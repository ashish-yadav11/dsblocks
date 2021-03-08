PREFIX = /usr/local

CC = gcc
CFLAGS = -O3 -Wall -Wextra

X11CFLAGS = $(shell pkg-config --cflags x11)
X11LIBS = $(shell pkg-config --libs x11)

BLOCKS := $(wildcard blocks/*.c)

all: dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

dsblocks.o: dsblocks.c shared.h config.h block.h
	${CC} -o $@ -c -Wno-missing-field-initializers -Wno-unused-parameter ${CFLAGS} ${X11CFLAGS} $<

util.o: util.c util.h shared.h
	${CC} -o $@ -c ${CFLAGS} ${X11CFLAGS} $<

blocks/%.o: blocks/%.c blocks/%.h util.h shared.h
	${CC} -o $@ -c -Wno-unused-parameter ${CFLAGS} $<

dsblocks: dsblocks.o util.o ${BLOCKS:c=o}
	${CC} -o $@ $^ ${X11LIBS}

sigdsblocks/sigdsblocks: sigdsblocks/sigdsblocks.c
	${CC} -o $@ ${CFLAGS} $<

xgetrootname/xgetrootname: xgetrootname/xgetrootname.c
	${CC} -o $@ ${CFLAGS} ${X11CFLAGS} $< ${X11LIBS}

clean:
	rm -f blocks/*.o *.o dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

BINDIR = ${DESTDIR}${PREFIX}/bin

install: all
	mkdir -p ${BINDIR}
	cp -f dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname ${BINDIR}
	chmod 755 ${BINDIR}/dsblocks ${BINDIR}/sigdsblocks ${BINDIR}/xgetrootname

uninstall:
	rm -f ${BINDIR}/dsblocks ${BINDIR}/sigdsblocks ${BINDIR}/xgetrootname

.PHONY: all clean install uninstall
