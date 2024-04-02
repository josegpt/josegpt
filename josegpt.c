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

#include <sys/queue.h>

#include <err.h>
#include <json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HTML_IMPLEMENTATION
#include "html.h"

SIMPLEQ_HEAD(projecthead, project);
struct project {
	char	              *name;
	char	              *desc;
	char	              *url;
	char	              *lic;
	char	              *lang;
	SIMPLEQ_ENTRY(project) projects;
};

struct conv {
	const char	*from;
	const char	*to;
};

int	getprojects(void);
void	freeproject(struct project *);
int	binsearch(const char *, struct conv *);

static struct projecthead head = SIMPLEQ_HEAD_INITIALIZER(head);

static struct conv convs[] = {
	{"C",           "c"},
	{"Emacs Lisp",  "elisp"},
	{"GPL-3.0",     "gpl3"},
	{"HTML",        "html"},
	{"ISC",         "isc"},
	{"MIT",         "mit"},
	{"Makefile",    "makefile"},
	{"NOASSERTION", "other"},
	{"Shell",       "shell"},
	{"Vue",         "js"},
};

int
getprojects(void)
{
	struct json_object *json, *o, *name, *desc, *url;
	struct json_object *lic, *lang, *spdx;
	struct project     *p;
	const char         *filename;
	int                 i, m, n, result;

	filename = "/cache/projects.json";
	json = json_object_from_file(filename);
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
			if (p) {
				p->name = strdup(name ? json_object_get_string(name) : "noname");
				p->desc = strdup(desc ? json_object_get_string(desc) : "nodescription");
				p->url  = strdup(url ? json_object_get_string(url) : "nourl");

				m = binsearch(spdx ? json_object_get_string(spdx) : "", convs);
				p->lic = strdup(m == -1 ? "none" : (convs + m)->to);

				m = binsearch(lang ? json_object_get_string(lang) : "", convs);
				p->lang = strdup(m == -1 ? "unknown" : (convs + m)->to);

				SIMPLEQ_INSERT_TAIL(&head, p, projects);
			} else {
				warn(NULL);
				result = 0;
				break;
			}
		}
		json_object_put(json);
		json = NULL;
		result = !SIMPLEQ_EMPTY(&head);
	} else {
		warn("could not read %s", filename);
		result = 0;
	}
	return (result);
}


void
freeproject(struct project *p)
{
	free(p->name);
	free(p->desc);
	free(p->url);
	free(p->lic);
	free(p->lang);
	free(p);
	p = NULL;
}

int
binsearch(const char *w, struct conv *cc)
{
	int cond, high, mid, low;

	low = 0;
	high = sizeof(convs) / sizeof(convs[0]) - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(w, cc[mid].from)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

int
main(void)
{
	struct html	 html;
	struct project	*p;

	if (pledge("stdio rpath", NULL) == -1)
		err(EXIT_FAILURE, "pledge");

	puts("Status: 200 OK\r");
	puts("Content-Type: text/html\r");
	puts("\r");

	html_begin(&html);

	html_beginhead(&html);

	html_beginmeta(&html);
	html_begincharset(&html);
	html_text(&html, "utf-8");
	html_endcharset(&html);
	html_endmeta(&html);

	html_begintitle(&html);
	html_text(&html, "josegpt &sol; %s", "projects");
	html_endtitle(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "width=device-width, initial-scale=1");
	html_endcontent(&html);
	html_beginname(&html);
	html_text(&html, "viewport");
	html_endname(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "josegpt");
	html_endcontent(&html);
	html_beginname(&html);
	html_text(&html, "author");
	html_endname(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "software engineer");
	html_endcontent(&html);
	html_beginname(&html);
	html_text(&html, "description");
	html_endname(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "C, HTML, CSS, JSON");
	html_endcontent(&html);
	html_beginname(&html);
	html_text(&html, "keywords");
	html_endname(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "red");
	html_endcontent(&html);
	html_beginname(&html);
	html_text(&html, "theme-color");
	html_endname(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "website");
	html_endcontent(&html);
	html_beginproperty(&html);
	html_text(&html, "og:type");
	html_endproperty(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "josegpt &sol; %s", "projects");
	html_endcontent(&html);
	html_beginproperty(&html);
	html_text(&html, "og:title");
	html_endproperty(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "https://josegpt.com");
	html_endcontent(&html);
	html_beginproperty(&html);
	html_text(&html, "og:url");
	html_endproperty(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "software engineer");
	html_endcontent(&html);
	html_beginproperty(&html);
	html_text(&html, "og:description");
	html_endproperty(&html);
	html_endmeta(&html);

	html_beginmeta(&html);
	html_begincontent(&html);
	html_text(&html, "/static/img/banner.png");
	html_endcontent(&html);
	html_beginproperty(&html);
	html_text(&html, "og:img");
	html_endproperty(&html);
	html_endmeta(&html);

	html_beginlink(&html);
	html_beginrel(&html);
	html_text(&html, "icon");
	html_endrel(&html);
	html_beginhref(&html);
	html_text(&html, "/static/img/favicon.ico");
	html_endhref(&html);
	html_begintype(&html);
	html_text(&html, "img/x-icon");
	html_endtype(&html);
	html_endlink(&html);

	html_beginlink(&html);
	html_beginrel(&html);
	html_text(&html, "preload");
	html_text(&html, "stylesheet");
	html_endcontent(&html);
	html_beginhref(&html);
	html_text(&html, "/static/css/style.css");
	html_endhref(&html);
	html_begintype(&html);
	html_text(&html, "text/css");
	html_endtype(&html);
	html_beginas(&html);
	html_text(&html, "style");
	html_endas(&html);
	html_endlink(&html);

	html_endhead(&html);

	html_beginbody(&html);
	html_beginclass(&html);
	html_text(&html, "cover");
	html_endclass(&html);

	html_beginheader(&html);

	html_beginnav(&html);
	html_beginclass(&html);
	html_text(&html, "cluster");
	html_endclass(&html);

	html_begindata(&html, "align");
	html_text(&html, "center");
	html_enddata(&html);

	html_beginanchor(&html);
	html_beginclass(&html);
	html_text(&html, "cluster");
	html_endclass(&html);

	html_beginhref(&html);
	html_text(&html, "/");
	html_endhref(&html);

	html_beginimg(&html);
	html_beginheight(&html);
	html_text(&html, "%d", 30);
	html_endheight(&html);

	html_beginwidth(&html);
	html_text(&html, "%d", 50);
	html_endwidth(&html);

	html_beginsrc(&html);
	html_text(&html, "/static/img/logo.svg");
	html_endsrc(&html);

	html_beginalt(&html);
	html_text(&html, "Logo");
	html_endalt(&html);

	html_endimg(&html);
	html_endanchor(&html);

	html_begindiv(&html);

	html_beginspan(&html);
	html_text(&html, "projects");
	html_endspan(&html);

	html_enddiv(&html);
	html_endnav(&html);
	html_endheader(&html);

	if (getprojects()) {
		html_beginmain(&html);
		html_beginclass(&html);
		html_text(&html, "stack");
		html_endclass(&html);

		html_beginul(&html);
		while ((p = SIMPLEQ_FIRST(&head))) {
			html_beginli(&html);

			html_beginanchor(&html);
			html_beginhref(&html);
			html_text(&html, p->url);
			html_endhref(&html);
			html_begintarget(&html);
			html_text(&html, "_blank");
			html_endtarget(&html);
			html_beginrel(&html);
			html_text(&html, "noopener");
			html_text(&html, "noreferrer");
			html_endrel(&html);
			html_text(&html, p->name);

			html_endanchor(&html);
			html_text(&html, " ");
			html_begincode(&html);
			html_text(&html, "%s@%s", p->lang, p->lic);
			html_endcode(&html);
			html_text(&html, ": %s", p->desc);
			html_endli(&html);

			SIMPLEQ_REMOVE_HEAD(&head, projects);
			freeproject(p);
		}
		html_endul(&html);

		html_endmain(&html);
	} else {
		html_beginmain(&html);
		html_beginclass(&html);
		html_text(&html, "cluster");
		html_endclass(&html);

		html_begindata(&html, "justify");
		html_text(&html, "center");
		html_enddata(&html);

		html_begindata(&html, "align");
		html_text(&html, "center");
		html_enddata(&html);

		html_beginsection(&html);
		html_beginclass(&html);
		html_text(&html, "text-align:center");
		html_endclass(&html);

		html_beginh1(&html);
		html_text(&html, "No Projects Found");
		html_endh1(&html);

		html_endsection(&html);
		html_endmain(&html);
	}

	html_beginfooter(&html);
	html_beginclass(&html);
	html_text(&html, "stack");
	html_endclass(&html);

	html_beginnav(&html);
	html_beginclass(&html);
	html_text(&html, "cluster");
	html_endclass(&html);

	html_begindata(&html, "justify");
	html_text(&html, "center");
	html_enddata(&html);

	html_beginanchor(&html);
	html_beginhref(&html);
	html_text(&html, "mailto:josegpt27@gmail.com");
	html_endhref(&html);
	html_text(&html, "Email");
	html_endanchor(&html);

	html_beginanchor(&html);
	html_beginhref(&html);
	html_text(&html, "https://sr.ht/~josegpt/");
	html_endhref(&html);
	html_begintarget(&html);
	html_text(&html, "_blank");
	html_endtarget(&html);
	html_beginrel(&html);
	html_text(&html, "noopener");
	html_text(&html, "noreferrer");
	html_endrel(&html);
	html_text(&html, "Sourcehut");
	html_endanchor(&html);

	html_beginanchor(&html);
	html_beginhref(&html);
	html_text(&html, "https://github.com/josegpt");
	html_endhref(&html);
	html_begintarget(&html);
	html_text(&html, "_blank");
	html_endtarget(&html);
	html_beginrel(&html);
	html_text(&html, "noopener");
	html_text(&html, "noreferrer");
	html_endrel(&html);
	html_text(&html, "GitHub");
	html_endanchor(&html);

	html_endnav(&html);

	html_beginp(&html);
	html_beginclass(&html);
	html_text(&html, "cluster");
	html_endclass(&html);

	html_begindata(&html, "justify");
	html_text(&html, "center");
	html_enddata(&html);

	html_begindata(&html, "align");
	html_text(&html, "center");
	html_enddata(&html);

	html_beginsmall(&html);
	html_text(&html, "&copy2017 ");
	html_text(&html, "josegpt&commat;");
	html_beginem(&html);
	html_text(&html, VERSION);
	html_endem(&html);
	html_endsmall(&html);

	html_endfooter(&html);
	html_endbody(&html);
	html_end(&html);

	return (EXIT_SUCCESS);
}
