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

#include <json.h>
#include <string.h>

#include "vc.h"

#define nitems(aa) (sizeof(aa) / sizeof(aa[0]))

struct licenserow {
	const char  *from;
	const char  *to;
	enum license license;
};

struct languagerow {
	const char   *from;
	const char   *to;
	enum language language;
};

static struct licenserow licensetable[] = {
	{"ISC",     "isc",  ISC},
	{"MIT",     "mit",  MIT},
	{"GPL-3.0", "gpl3", GPL3},
};

static struct languagerow languagetable[] = {
	{"C",          "c",        C},
	{"HTML",       "html",     HTML_},
	{"Shell",      "shell",    SHELL},
	{"Makefile",   "makefile", MAKEFILE},
	{"Emacs Lisp", "elisp",    EMACSLISP},
};

static enum license  strlicense(const char *);
static enum language strlanguage(const char *);
static size_t        jsonprojects(struct project *);

size_t
vc_getprojects(struct project *project)
{
	return jsonprojects(project);
}

static const char *
licensestr(enum license a)
{
	struct licenserow b;
	size_t i;

	for (i = 0; i < nitems(licensetable); ++i) {
		b = licensetable[i];
		if (b.license == a)
			return b.to;
	}
	return "None";
}

static const char *
languagestr(enum language a)
{
	struct languagerow b;
	size_t i;

	for (i = 0; i < nitems(languagetable); ++i) {
		b = languagetable[i];
		if (b.language == a)
			return b.to;
	}
	return "Unknown";
}

static enum license
strlicense(const char *str)
{
	struct licenserow l;
	size_t i;

	for (i = 0; i < nitems(licensetable); ++i) {
		l = licensetable[i];
		if (strcmp(l.from, str) == 0)
			return l.license;
	}
	return NONE;
}

static enum language
strlanguage(const char *str)
{
	struct languagerow l;
	size_t i;

	for (i = 0; i < nitems(languagetable); ++i) {
		l = languagetable[i];
		if (strcmp(l.from, str) == 0)
			return l.language;
	}
	return UNKNOWN;
}

static size_t
jsonprojects(struct project *projects)
{
	struct json_object *jsonp, *project, *name, *description, *url;
	struct json_object *license, *spdxid, *language;
	struct project     *pp;
	const char         *str;
	size_t              i, n;

	jsonp = json_object_from_file("/cache/projects.json");
	if (jsonp) {
		n = json_object_array_length(jsonp);
		for (i = 0, pp = projects; i < n; ++i, ++pp) {
			project = json_object_array_get_idx(jsonp, i);
			if (json_object_object_get_ex(project, "name", &name)) {
				str = json_object_get_string(name);
				if (str) pp->name = strdup(str);
			}

			if (json_object_object_get_ex(project, "description", &description)) {
				str = json_object_get_string(description);
				if (str) pp->description = strdup(str);
			}

			if (json_object_object_get_ex(project, "html_url", &url)) {
				str = json_object_get_string(url);
				if (str) pp->url = strdup(str);
			}

			if (json_object_object_get_ex(project, "license", &license)) {
				if (json_object_object_get_ex(license, "spdx_id", &spdxid)) {
					str = json_object_get_string(spdxid);
					if (str) pp->license = strlicense(str);
					else     pp->license = NONE;
				}
			}

			if (json_object_object_get_ex(project, "language", &language)) {
				str = json_object_get_string(language);
				if (str) pp->language = strlanguage(str);
				else     pp->language = UNKNOWN;
			}
		}
		json_object_put(jsonp);
		jsonp = NULL;
	} else {
		n = 0;
	}
	return n;
}

