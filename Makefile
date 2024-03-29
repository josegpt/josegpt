VERSION?=	dev

PREFIX=		${DESTDIR}/var/www
CGIDIR=		${PREFIX}/cgi-bin
HTDOCSDIR=	${PREFIX}/htdocs/josegpt

JSONINCS=	/usr/local/include/json-c
JSONLIB=	/usr/local/lib

INCS=		-I${JSONINCS}
LIBS=		-L${JSONLIB} -ljson-c

CFLAGS=		-O2 -g -W -Wall -Werror -Wextra -pedantic ${INCS}
LDFLAGS=	-static -pie ${LIBS}

all: josegpt

.c.o:
	${CC} -DVERSION=\"${VERSION}\" ${CFLAGS} -c $<

josegpt: josegpt.o
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
