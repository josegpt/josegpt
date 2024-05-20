PREFIX=		${DESTDIR}/var/www
CGIDIR=		${PREFIX}/cgi-bin

INCS=		-I/usr/local/include
LIBS=		-L/usr/local/lib -lcjson -lhtml

CFLAGS=		-O2 -g -W -Wall -Werror -Wextra -pedantic -ansi ${INCS}
LDFLAGS=	-static -pie ${LIBS}

all: josegpt

.c.o:
	${CC} ${CFLAGS} -c $<

josegpt: josegpt.o
	${CC} -o $@ ${LDFLAGS} $?

clean:
	rm -rf *.o *.core ktrace.out josegpt

install: all
	mkdir -p ${CGIDIR}
	cp -p josegpt ${CGIDIR}

uninstall:
	rm -rf ${CGIDIR}/josegpt

.PHONY: all clean install uninstall
