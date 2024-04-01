/*
 * Copyright (c) 2024 Jose G Perez Taveras <josegpt27@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef VC_H
#define VC_H

#include <sys/queue.h>

enum lic {
	NONE,
	ISC,
	MIT,
	GPL3
};

enum lang {
	UNKNOWN,
	C,
	HTML_,
	SHELL,
	MAKEFILE,
	EMACSLISP,
	JAVASCRIPT
};

struct project {
	char                  *name;
	char                  *desc;
	char                  *url;
	enum lic               lic;
	enum lang              lang;
	SIMPLEQ_ENTRY(project) projects;
};

SIMPLEQ_HEAD(portfolio, project);

int         getprojects(struct portfolio *);
void        freeproject(struct project *);
const char *lictostr(enum lic);
const char *langtostr(enum lang);

#endif /* VC_H */
