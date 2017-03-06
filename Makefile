# wmcontrib - Collection of C programs to help with no-wm X use.
# © 2016 Julian Speedie

include config.mk

SRC = \
	wtfc.c \
	wtsr.c
OBJ = ${SRC:.c=.o}
BIN = ${SRC:.c=}
MAN = $(SRC:.c=.1.gz)

all: options $(BIN)

options:
	@echo wmcontrib build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c $< -o $@ ${CFLAGS} ${LDFLAGS}

${OBJ}: config.mk

install: $(BIN)
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f $(BIN) ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtfc
	@chmod u+s ${DESTDIR}${PREFIX}/bin/wtfc
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtsr
	@chmod u+s ${DESTDIR}${PREFIX}/bin/wtsr
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

clean:
	@echo cleaning
	@rm -f wtfc ${OBJ} wtfc-${VERSION}.tar.gz

.PHONY: all options clean install uninstall
