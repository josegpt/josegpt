PREFIX=		${DESTDIR}/var/www
CGIDIR=		${PREFIX}/cgi-bin

HTMLINC=	/usr/local/include
HTMLLIB=	/usr/local/lib

JSONINC=	/usr/local/include/json-c
JSONLIB=	/usr/local/lib

INCS=		-I${JSONINC} -I${HTMLINC}
LIBS=		-L${JSONLIB} -ljson-c -L${HTMLLIB} -lhtml

CFLAGS=		-O2 -g -W -Wall -Werror -Wextra -pedantic ${INCS}
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
