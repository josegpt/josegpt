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

enum license {
	NONE,
	ISC,
	MIT,
	GPL3
};

enum language {
	UNKNOWN,
	C,
	SHELL,
	EMACSLISP
};

struct project {
	const char   *name;
	const char   *description;
	const char   *url;
	enum license  license;
	enum language language;
};

struct portfolio {
	struct project projects[32];
	size_t         howmany;
};

#include "github.h"

size_t      vc_get(struct project *);
const char *vc_licensestr(enum license);
const char *vc_languagestr(enum language);

#endif /* VC_H */
