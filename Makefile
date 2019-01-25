# wmcontrib - Collection of C programs to help with no-wm X use.
# © 2016 Julian Speedie

# wmcontrib version
VERSION = 0.5
# Paths
PREFIX = /usr/local
MANPREFIX = /usr/share/man
# Special Includes
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib
# gcc flags for includes
INCS = -I. -I/usr/include -I${X11INC}
LIBS = -L/usr/lib -lc -lcrypt -L${X11LIB} -lX11 -lXext -lXrandr
# Flags
CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS = -s ${LIBS}
# Compiler and linker
# CC = gcc -ggdb
CC = gcc

SRC = \
	wtfc.c \
	wtsr.c \
	wrsr.c \
	cfw.c \
	wsft.c \
	wrsz.c \
	wuc.c \
	wucm.c \
	wucr.c \
	wsd.c \
	scinf.c
DEP = common.c
OBJ = ${SRC:.c=.o}
BIN = ${SRC:.c=}
MAN = $(SRC:.c=.1.gz)

all: wtfc wtsr wrsr cfw wsft wrsz wuc wucm wsd scinf install

wtfc: wtfc.c $(DEP)
	$(CC) wtfc.c $(DEP) -o wtfc $(CFLAGS) $(LDFLAGS)

wtsr: wtsr.c $(DEP)
	$(CC) wtsr.c $(DEP) -o wtsr $(CFLAGS) $(LDFLAGS)

wrsr: wrsr.c $(DEP)
	$(CC) wrsr.c $(DEP) -o wrsr $(CFLAGS) $(LDFLAGS)

cfw: cfw.c $(DEP)
	$(CC) cfw.c $(DEP) -o cfw $(CFLAGS) $(LDFLAGS)

wsft: wsft.c $(DEP)
	$(CC) wsft.c $(DEP) -o wsft $(CFLAGS) $(LDFLAGS)

wrsz: wrsz.c $(DEP)
	$(CC) wrsz.c $(DEP) -o wrsz $(CFLAGS) $(LDFLAGS)

wuc: wuc.c $(DEP)
	$(CC) wuc.c $(DEP) -o wuc $(CFLAGS) $(LDFLAGS)

wucm: wucm.c $(DEP)
	$(CC) wucm.c $(DEP) -o wucm $(CFLAGS) $(LDFLAGS)

wucr: wucr.c $(DEP)
	$(CC) wucr.c $(DEP) -o wucr $(CFLAGS) $(LDFLAGS)

wsd: wsd.c $(DEP)
	$(CC) wsd.c $(DEP) -o wsd $(CFLAGS) $(LDFLAGS)

scinf: scinf.c $(DEP)
	$(CC) scinf.c $(DEP) -o scinf $(CFLAGS) $(LDFLAGS)

install: $(BIN)
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f $(BIN) ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtfc
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtsr
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wrsr
	@chmod 755 ${DESTDIR}${PREFIX}/bin/cfw
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wsft
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wrsz
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wuc
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wucm
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wucr
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wsd
	@chmod 755 ${DESTDIR}${PREFIX}/bin/scinf
	@mkdir -p $(MANPREFIX)/man1/
	@cp -f $(MAN) $(MANPREFIX)/man1/

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@for wmcontrib in $(BIN); do \
		rm -f ${DESTDIR}${PREFIX}/bin/$$wmcontrib; \
	done
	@for page in $(MAN); do \
		rm -f $(MANPREFIX)/man1/$$page; \
	done

# Needs to be fixed
clean:
	@echo cleaning
	@rm -f wtfc ${OBJ} wtfc-${VERSION}.tar.gz

# .PHONY: all clean install uninstall
