VERSION=	0.0

PREFIX=		${DESTDIR}/var/www
CGIDIR=		${PREFIX}/cgi-bin
HTDOCSDIR=	${PREFIX}/htdocs/josegpt

CFLAGS=		-O2 -W -Wall -Werror -Wextra -pedantic -ansi -g
LDFLAGS=	-static -pie

all: josegpt

.c.o:
	${CC} -DVERSION=${VERSION} ${CFLAGS} -c $<

josegpt: html.o
	${CC} -o $@ ${LDFLAGS} $?

clean:
	rm -rf *.o *.core josegpt

install: all
	mkdir -p ${CGIDIR} ${HTDOCSDIR}
	cp -Rp josegpt ${CGIDIR}
	cp -Rp logo.svg favicon.ico style.css ${HTDOCSDIR}

uninstall:
	rm -rf ${CGIDIR}/josegpt ${HTDOCSDIR}

.PHONY: all clean static install uninstall
