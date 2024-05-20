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

#include <cjson/cJSON.h>
#include <err.h>
#include <html.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct project {
	char		*name;
	char		*desc;
	char		*url;
	char		*lic;
	char		*lang;
	struct project	*next;
};

struct project	*getpp(char *);
struct project	*createp(const char *, const char *, const char *,
    const char *, const char *);
void		 freep(struct project *);
char		*readfile(const char *);
const char	*map(const char *);
void		*ecalloc(size_t, size_t);
char		*estrdup(const char *);

static struct {
	const char	*from;
	const char	*to;
} mm[] = {
	{"C",		"c"},
	{"CSS",		"css"},
	{"Emacs Lisp",	"elisp"},
	{"GPL-3.0",	"gpl3"},
	{"HTML",	"html"},
	{"ISC",		"isc"},
	{"MIT",		"mit"},
	{"Makefile",	"makefile"},
	{"NOASSERTION",	"other"},
	{"Shell",	"shell"},
	{"Vue",		"js"}
};

struct project *
getpp(char *s)
{
	struct project *p, *ph, **pt;
	cJSON *json, *desc, *lang, *lic;
	cJSON *name, *o, *spdx, *url;

	ph = NULL;
	pt = &ph;
	json = cJSON_Parse(s);
	if (json) {
		cJSON_ArrayForEach(o, json) {
			name = cJSON_GetObjectItemCaseSensitive(o, "name");
			desc = cJSON_GetObjectItemCaseSensitive(o, "description");
			url  = cJSON_GetObjectItemCaseSensitive(o, "html_url");
			lang = cJSON_GetObjectItemCaseSensitive(o, "language");
			lic  = cJSON_GetObjectItemCaseSensitive(o, "license");
			if (cJSON_IsObject(o))
				spdx  = cJSON_GetObjectItemCaseSensitive(lic, "spdx_id");
			p = createp(cJSON_IsString(name) ? name->valuestring : "noname",
			    cJSON_IsString(desc) ? desc->valuestring : "nodesc",
			    cJSON_IsString(url)  ? url->valuestring  : "nourl",
			    cJSON_IsString(spdx) ? spdx->valuestring  : "nolic",
			    cJSON_IsString(lang) ? lang->valuestring : "nolang");
			*pt = p;
			pt = &p->next;
		}
		cJSON_Delete(json);
	} else
		warn("could not parse json: %s", cJSON_GetErrorPtr());
	return (ph);
}

struct project *
createp(const char *name, const char *desc, const char *url,
    const char *lic, const char *lang)
{
	struct project *p;
	const char *s;

	p = (struct project *)ecalloc(1, sizeof(struct project));
	p->name = estrdup(name);
	p->desc = estrdup(desc);
	p->url  = estrdup(url);
	p->lic  = estrdup((s = map(lic))  ? s : "none");
	p->lang = estrdup((s = map(lang)) ? s : "unknown");
	return (p);
}

void
freep(struct project *p)
{
	free(p->name);
	free(p->desc);
	free(p->url);
	free(p->lic);
	free(p->lang);
	free(p);
}

char *
readfile(const char *filename)
{
	FILE *fp;
	char *result;
	long count;

	result = NULL;
	fp = fopen(filename, "r");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		count = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		result = (char *)ecalloc(1, count + 1);
		fread(result, count, 1, fp);
		fclose(fp);
	}
	return (result);
}

const char *
map(const char *w)
{
	int cond, high, mid, low;

	low = 0;
	high = sizeof(mm) / sizeof(mm[0]) - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(w, mm[mid].from)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return (mm[mid].to);
	}
	return (NULL);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if ((p = calloc(nmemb, size)) == NULL)
		err(1, NULL);
	return (p);
}

char *
estrdup(const char *s)
{
	char *p;

	if ((p = strdup(s)) == NULL)
		err(1, NULL);
	return (p);
}

int
main(void)
{
	static struct html html;
	struct project *p, *t;
	char *json;

	if (pledge("stdio rpath", NULL) == -1)
		err(EXIT_FAILURE, "pledge");

	json = readfile("/cache/projects.json");

	puts("Status: 200 OK\r");
	puts("Content-Type: text/html\r");
	puts("\r");

	html_begin(&html);
	html_beginlang(&html);
	html_text(&html, "en-US");
	html_endlang(&html);

	html_beginhead(&html);
	html_beginmeta(&html);
	html_begincharset(&html);
	html_text(&html, "utf-8");
	html_endcharset(&html);
	html_endmeta(&html);

	html_begintitle(&html);
	html_text(&html, "josegpt &sol; Projects");
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
	html_text(&html, "#d90000");
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
	html_text(&html, "josegpt &sol; Projects");
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
	html_text(&html, "og:image");
	html_endproperty(&html);
	html_endmeta(&html);

	html_beginlink(&html);
	html_beginrel(&html);
	html_text(&html, "icon");
	html_endrel(&html);
	html_beginhref(&html);
	html_text(&html, "/static/img/logo.svg");
	html_endhref(&html);
	html_endlink(&html);

	html_beginlink(&html);
	html_beginrel(&html);
	html_text(&html, "mask-icon");
	html_endrel(&html);
	html_beginhref(&html);
	html_text(&html, "/static/img/logo.svg");
	html_endhref(&html);
	html_begincolor(&html);
	html_text(&html, "#d90000");
	html_endcolor(&html);
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

	html_begina(&html);
	html_begindata(&html, "style");
	html_text(&html, "logo");
	html_enddata(&html);
	html_beginhref(&html);
	html_text(&html, "/");
	html_endhref(&html);
	html_beginimg(&html);
	html_beginheight(&html);
	html_text(&html, "30");
	html_endheight(&html);
	html_beginwidth(&html);
	html_text(&html, "30");
	html_endwidth(&html);
	html_beginsrc(&html);
	html_text(&html, "/static/img/logo.svg");
	html_endsrc(&html);
	html_beginalt(&html);
	html_text(&html, "Logo");
	html_endalt(&html);
	html_endimg(&html);
	html_enda(&html);
	html_begindiv(&html);

	html_beginspan(&html);
	html_text(&html, "Projects");
	html_endspan(&html);

	html_enddiv(&html);
	html_endnav(&html);
	html_endheader(&html);

	if ((p = getpp(json))) {
		html_beginmain(&html);
		html_beginclass(&html);
		html_text(&html, "stack");
		html_endclass(&html);
		for (; p && (t = p->next, 1); p = t) {
			html_beginarticle(&html);
			html_begina(&html);
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
			html_enda(&html);
			html_text(&html, " ");
			html_begincode(&html);
			html_text(&html, p->lang);
			html_endcode(&html);
			html_text(&html, " ");
			html_begincode(&html);
			html_text(&html, p->lic);
			html_endcode(&html);
			html_beginp(&html);
			html_text(&html, p->desc);
			html_endp(&html);
			html_endarticle(&html);
			freep(p);
		}
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

	html_begina(&html);
	html_beginhref(&html);
	html_text(&html, "mailto:josegpt27@gmail.com");
	html_endhref(&html);
	html_text(&html, "Email");
	html_enda(&html);

	html_begina(&html);
	html_beginhref(&html);
	html_text(&html, "https://design.redhex.dev");
	html_endhref(&html);
	html_text(&html, "Design");
	html_enda(&html);

	html_begina(&html);
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
	html_enda(&html);

	html_begina(&html);
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
	html_enda(&html);

	html_endnav(&html);

	html_beginp(&html);
	html_beginclass(&html);
	html_text(&html, "text-align:center");
	html_endclass(&html);

	html_endfooter(&html);
	html_endbody(&html);
	html_end(&html);

	return (EXIT_SUCCESS);
}
