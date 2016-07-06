# wtfc - cardinal window focus transferrer
# © 2016 Julian Speedie

include config.mk

SRC = wtfc.c
OBJ = ${SRC:.c=.o}
MAN = wtfc.1.gz

all: options wtfc

options:
	@echo wtfc build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

wtfc: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f wtfc ${OBJ} wtfc-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p wtfc-${VERSION}
	@cp -R LICENSE Makefile README config.mk ${SRC} wtfc-${VERSION}
	@tar -cf wtfc-${VERSION}.tar wtfc-${VERSION}
	@gzip wtfc-${VERSION}.tar
	@rm -rf wtfc-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f wtfc ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/wtfc
	@chmod u+s ${DESTDIR}${PREFIX}/bin/wtfc
	@mkdir -p $(MANPREFIX)/man1/
	@cp -f $(MAN) $(MANPREFIX)/man1/

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/wtfc
	@for page in $(MAN); do \
		rm -f $(MANPREFIX)/man1/$$page; \
	done

.PHONY: all options clean dist install uninstall
