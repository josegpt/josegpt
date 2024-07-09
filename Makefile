PREFIX?=	${DESTDIR}/var
HTDOCSDIR=	${PREFIX}/www/htdocs
STATICDIR=	${HTDOCSDIR}/josegpt

install:
	install -d ${STATICDIR}
	install -m 0644 *.html ${STATICDIR}

uninstall:
	rm -rf ${STATICDIR}

.PHONY: install uninstall
