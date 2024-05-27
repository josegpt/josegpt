PREFIX?=	${DESTDIR}/var/www
HTDOCSDIR=	${PREFIX}/htdocs
STATICDIR=	${HTDOCSDIR}/josegpt

all: static

static:
	mkdir -p ${STATICDIR}
	cp -Rp *.html ${STATICDIR}

install: all

uninstall:
	rm -rf ${STATICDIR}

.PHONY: all static install uninstall
