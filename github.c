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

static enum license
strlicense(const char *str)
{
	enum license result;
	if      (strcmp("ISC", str) == 0)     result = ISC;
	else if (strcmp("MIT", str) == 0)     result = MIT;
	else if (strcmp("GPL-3.0", str) == 0) result = GPL3;
	else                                  result = NONE;
	return result;
}

static enum language
strlanguage(const char *str)
{
	enum language result;
	if      (strcmp("C", str) == 0)          result = C;
	else if (strcmp("HTML", str) == 0)       result = HTML_;
	else if (strcmp("Shell", str) == 0)      result = SHELL;
	else if (strcmp("Makefile", str) == 0)   result = MAKEFILE;
	else if (strcmp("Emacs Lisp", str) == 0) result = EMACSLISP;
	else                                     result = UNKNOWN;
	return result;
}

size_t
github_get(struct project *projects)
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
