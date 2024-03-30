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

#include "vc.h"
#include "github.c"

size_t
vc_get(struct project *project)
{
	return github_get(project);
}

const char *
licensestr(enum license l)
{
	const char *result;
	switch (l) {
	case ISC:
		result = "ISC";
	break;
	case MIT:
		result = "MIT";
	break;
	case GPL3:
		result = "GPL3";
	break;
	default:
		result = "None";
	}
	return result;
}

const char *
languagestr(enum language l)
{
	const char *result;
	switch (l) {
	case C:
		result = "C";
	break;
	case HTML_:
		result = "HTML";
	break;
	case SHELL:
		result = "Shell";
	break;
	case MAKEFILE:
		result = "Makefile";
	break;
	case EMACSLISP:
		result = "Elisp";
	break;
	default:
		result = "Unknown";
	}
	return result;
}
