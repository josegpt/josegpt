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

static struct {
	const char *from;
	enum lic    to;
} lictable[] = {
	{"ISC",     ISC},
	{"MIT",     MIT},
	{"GPL-3.0", GPL3}
};

static struct {
	const char *from;
	enum lang   to;
} langtable[] = {
	{"C",          C},
	{"HTML",       HTML_},
	{"Shell",      SHELL},
	{"Makefile",   MAKEFILE},
	{"Emacs Lisp", EMACSLISP},
	{"Vue",        JAVASCRIPT}
};

static enum lic  strtolic(const char *);
static enum lang strtolang(const char *);

int
getprojects(struct portfolio *pp)
{
	struct json_object *json, *o, *name, *desc, *url;
	struct json_object *lic, *lang, *spdx;
	struct project     *p;
	int                 i, n;

	json = json_object_from_file("projects.json");
	if (json) {
		n = json_object_array_length(json);
		for (i = 0; i < n; ++i) {
			o = json_object_array_get_idx(json, i);

			json_object_object_get_ex(o, "name", &name);
			json_object_object_get_ex(o, "description", &desc);
			json_object_object_get_ex(o, "html_url", &url);
			json_object_object_get_ex(o, "license", &lic);
			json_object_object_get_ex(o, "language", &lang);

			json_object_object_get_ex(lic, "spdx_id", &spdx);

			p = malloc(sizeof(struct project));
			p->name = strdup(name ? json_object_get_string(name) : "noname");
			p->desc = strdup(desc ? json_object_get_string(desc) : "nodescription");
			p->url  = strdup(url ? json_object_get_string(url) : "nourl");
			p->lic  = spdx ? strtolic(json_object_get_string(spdx)) : NONE;
			p->lang = lang ? strtolang(json_object_get_string(lang)) : UNKNOWN;

			SIMPLEQ_INSERT_TAIL(pp, p, projects);
		}

		json_object_put(json);
		json = NULL;
	}

	return SIMPLEQ_EMPTY(pp);
}


void
freeproject(struct project *p)
{
	free(p->name);
	free(p->desc);
	free(p->url);
	free(p);
}

const char *
lictostr(enum lic l)
{
	char *result;

	switch (l) {
	case ISC:
		result = "isc";
		break;
	case MIT:
		result = "mit";
		break;
	case GPL3:
		result = "gpl3";
		break;
	default:
		result = "none";
	}
	return (result);
}

const char *
langtostr(enum lang l)
{
	char *result;

	switch (l) {
	case C:
		result = "c";
		break;
	case HTML_:
		result = "html";
		break;
	case SHELL:
		result = "shell";
		break;
	case MAKEFILE:
		result = "makefile";
		break;
	case EMACSLISP:
		result = "elisp";
		break;
	case JAVASCRIPT:
		result = "js";
		break;
	default:
		result = "unknown";
	}
	return (result);
}

static enum lic
strtolic(const char *str)
{
	int i;

	for (i = 0; i < (int)nitems(lictable); ++i) {
		if (strcmp(lictable[i].from, str) == 0)
			return (lictable[i].to);
	}
	return (NONE);
}

static enum lang
strtolang(const char *str)
{
	int i;

	for (i = 0; i < (int)nitems(langtable); ++i) {
		if (strcmp(langtable[i].from, str) == 0)
			return (langtable[i].to);
	}
	return (UNKNOWN);
}
