# wmcontrib - Collection of C programs to help with no-wm X use.
# © 2016 Julian Speedie

# wmcontrib version
VERSION = 0.3
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
	wtsr.c
DEP = common.c
OBJ = ${SRC:.c=.o}
BIN = ${SRC:.c=}
MAN = $(SRC:.c=.1.gz)

all: wtfc wtsr install

wtsr: wtsr.c $(DEP)
	$(CC) wtsr.c $(DEP) -o wtsr $(CFLAGS) $(LDFLAGS)

wtfc: wtfc.c $(DEP)
	$(CC) wtfc.c $(DEP) -o wtfc $(CFLAGS) $(LDFLAGS)

install: $(BIN)
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f $(BIN) ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtfc
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtsr
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
