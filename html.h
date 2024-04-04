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

#ifndef HTML_H
#define HTML_H

enum html_tag {
	HTML, HEAD, META, TITLE, LINK, BODY,
	HEADER, FOOTER, NAV, SECTION, MAIN, ARTICLE, DIV,
	H1, H2, H3, H4, H5, H6,
	P, HR, PRE, BLOCKQUOTE, ANCHOR, CODE,
	UL, OL, LI,
	SMALL, EM, IMG, SPAN,
	TAGMAX
};

enum html_attr {
	CHARSET, NAME, CONTENT, PROPERTY, REL,
	LANG, HREF, TYPE, AS, CLASS, DATA,
	HEIGHT, WIDTH, SRC, ALT, TARGET, COLOR,
	ATTRMAX
};

enum html_element {
	BEGIN,
	TAG,
	SELFTAG,
	ATTRKEY,
	ATTRVAL,
	TEXT
};

struct html {
	enum html_element state;
};

void html_begin(struct html *);
void html_begintag(struct html *, const char *);
void html_beginselftag(struct html *, const char *);
void html_beginattr(struct html *, const char *);
void html_beginhead(struct html *);
void html_begintitle(struct html *);
void html_beginbody(struct html *);
void html_beginmeta(struct html *);
void html_beginclass(struct html *);
void html_begindata(struct html *, const char *);
void html_begincharset(struct html *);
void html_begincontent(struct html *);
void html_beginname(struct html *);
void html_beginproperty(struct html *);
void html_beginlink(struct html *);
void html_beginrel(struct html *);
void html_beginlang(struct html *);
void html_beginhref(struct html *);
void html_begintype(struct html *);
void html_beginas(struct html *);
void html_beginheader(struct html *);
void html_begindiv(struct html *);
void html_beginmain(struct html *);
void html_beginarticle(struct html *);
void html_beginul(struct html *);
void html_beginol(struct html *);
void html_beginli(struct html *);
void html_beginp(struct html *);
void html_beginnav(struct html *);
void html_beginanchor(struct html *);
void html_begincode(struct html *);
void html_beginimg(struct html *);
void html_beginheight(struct html *);
void html_beginwidth(struct html *);
void html_beginsrc(struct html *);
void html_beginalt(struct html *);
void html_beginsection(struct html *);
void html_begintarget(struct html *);
void html_beginspan(struct html *);
void html_beginfooter(struct html *);
void html_beginsmall(struct html *);
void html_beginem(struct html *);
void html_beginh1(struct html *);
void html_beginh2(struct html *);
void html_beginh3(struct html *);
void html_beginh4(struct html *);
void html_beginh5(struct html *);
void html_beginh6(struct html *);

void html_end(struct html *);
void html_endtag(struct html *, const char *);
void html_endselftag(struct html *);
void html_endattr(struct html *);
void html_endhead(struct html *);
void html_endtitle(struct html *);
void html_endbody(struct html *);
void html_endmeta(struct html *);
void html_endclass(struct html *);
void html_enddata(struct html *);
void html_endcharset(struct html *);
void html_endcontent(struct html *);
void html_endname(struct html *);
void html_endproperty(struct html *);
void html_endlink(struct html *);
void html_endrel(struct html *);
void html_endlang(struct html *);
void html_endhref(struct html *);
void html_endtype(struct html *);
void html_endas(struct html *);
void html_endheader(struct html *);
void html_enddiv(struct html *);
void html_endmain(struct html *);
void html_endarticle(struct html *);
void html_endul(struct html *);
void html_endol(struct html *);
void html_endli(struct html *);
void html_endp(struct html *);
void html_endnav(struct html *);
void html_endanchor(struct html *);
void html_endcode(struct html *);
void html_endimg(struct html *);
void html_endheight(struct html *);
void html_endwidth(struct html *);
void html_endsrc(struct html *);
void html_endalt(struct html *);
void html_endsection(struct html *);
void html_endtarget(struct html *);
void html_endspan(struct html *);
void html_endfooter(struct html *);
void html_endsmall(struct html *);
void html_endem(struct html *);
void html_endh1(struct html *);
void html_endh2(struct html *);
void html_endh3(struct html *);
void html_endh4(struct html *);
void html_endh5(struct html *);
void html_endh6(struct html *);

void html_text(struct html *, const char *, ...);

#endif /* HTML_H */

#ifdef HTML_IMPLEMENTATION

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static const char *html_tags[TAGMAX] = {
	"html", "head", "meta", "title", "link", "body",
	"header", "footer", "nav", "section", "main", "article", "div",
	"h1", "h2", "h3", "h4", "h5", "h6",
	"p", "hr", "pre", "blockquote", "a", "code",
	"ul", "ol", "li",
	"small", "em", "img", "span"
};

static const char *html_attrs[ATTRMAX] = {
	"charset", "name", "content", "property", "rel",
	"lang", "href", "type", "as", "class", "data",
	"height", "width", "src", "alt", "target", "color"
};

static void
html_write(const char *str)
{
	fwrite(str, 1, strlen(str), stdout);
}

static void
html_writec(const int c)
{
	fputc(c, stdout);
}

static void
html_writef(const char *format, va_list ap)
{
	vfprintf(stdout, format, ap);
}

void
html_begin(struct html *html)
{
	html_write("<!doctype html>");
	html_begintag(html, html_tags[HTML]);
}

void
html_begintag(struct html *html, const char *str)
{
	if (html->state == TAG)
		html_writec('>');

	html_writec('<');
	html_write(str);
	html->state = TAG;
}

void
html_beginselftag(struct html *html, const char *str)
{
	if (html->state == TAG)
		html_writec('>');

	html_writec('<');
	html_write(str);
	html->state = SELFTAG;
}

void
html_beginattr(struct html *html, const char *str)
{
	if (html->state == TAG || html->state == SELFTAG)
		html_writec(' ');

	html_write(str);
	html_writec('=');
	html_writec('"');
	html->state = ATTRKEY;
}

void
html_beginhead(struct html *html)
{
	html_begintag(html, html_tags[HEAD]);
}

void
html_begintitle(struct html *html)
{
	html_begintag(html, html_tags[TITLE]);
}

void
html_beginbody(struct html *html)
{
	html_begintag(html, html_tags[BODY]);
}

void
html_beginmeta(struct html *html)
{
	html_beginselftag(html, html_tags[META]);
}

void
html_beginclass(struct html *html)
{
	html_beginattr(html, html_attrs[CLASS]);
}

void
html_begindata(struct html *html, const char *str)
{
	char buf[16];

	snprintf(buf, sizeof(buf), "%s-%s", html_attrs[DATA], str);
	html_beginattr(html, buf);
}

void
html_begincharset(struct html *html)
{
	html_beginattr(html, html_attrs[CHARSET]);
}

void
html_begincontent(struct html *html)
{
	html_beginattr(html, html_attrs[CONTENT]);
}

void
html_beginname(struct html *html)
{
	html_beginattr(html, html_attrs[NAME]);
}

void
html_beginproperty(struct html *html)
{
	html_beginattr(html, html_attrs[PROPERTY]);
}

void
html_beginlink(struct html *html)
{
	html_beginselftag(html, html_tags[LINK]);
}

void
html_beginrel(struct html *html)
{
	html_beginattr(html, html_attrs[REL]);
}

void
html_beginlang(struct html *html)
{
	html_beginattr(html, html_attrs[LANG]);
}

void
html_beginhref(struct html *html)
{
	html_beginattr(html, html_attrs[HREF]);
}

void
html_begintype(struct html *html)
{
	html_beginattr(html, html_attrs[TYPE]);
}

void
html_beginas(struct html *html)
{
	html_beginattr(html, html_attrs[AS]);
}

void
html_beginheader(struct html *html)
{
	html_begintag(html, html_tags[HEADER]);
}

void
html_begindiv(struct html *html)
{
	html_begintag(html, html_tags[DIV]);
}

void
html_beginmain(struct html *html)
{
	html_begintag(html, html_tags[MAIN]);
}

void
html_beginarticle(struct html *html)
{
	html_begintag(html, html_tags[ARTICLE]);
}

void
html_beginul(struct html *html)
{
	html_begintag(html, html_tags[UL]);
}

void
html_beginol(struct html *html)
{
	html_begintag(html, html_tags[OL]);
}

void
html_beginli(struct html *html)
{
	html_begintag(html, html_tags[LI]);
}

void
html_beginp(struct html *html)
{
	html_begintag(html, html_tags[P]);
}

void
html_beginnav(struct html *html)
{
	html_begintag(html, html_tags[NAV]);
}

void
html_beginanchor(struct html *html)
{
	html_begintag(html, html_tags[ANCHOR]);
}

void
html_begincode(struct html *html)
{
	html_begintag(html, html_tags[CODE]);
}

void
html_beginimg(struct html *html)
{
	html_beginselftag(html, html_tags[IMG]);
}

void
html_beginheight(struct html *html)
{
	html_beginattr(html, html_attrs[HEIGHT]);
}

void
html_beginwidth(struct html *html)
{
	html_beginattr(html, html_attrs[WIDTH]);
}

void
html_beginsrc(struct html *html)
{
	html_beginattr(html, html_attrs[SRC]);
}

void
html_beginalt(struct html *html)
{
	html_beginattr(html, html_attrs[ALT]);
}

void
html_beginsection(struct html *html)
{
	html_begintag(html, html_tags[SECTION]);
}

void
html_begintarget(struct html *html)
{
	html_beginattr(html, html_attrs[TARGET]);
}

void
html_begincolor(struct html *html)
{
	html_beginattr(html, html_attrs[COLOR]);
}

void
html_beginspan(struct html *html)
{
	html_begintag(html, html_tags[SPAN]);
}

void
html_beginfooter(struct html *html)
{
	html_begintag(html, html_tags[FOOTER]);
}

void
html_beginsmall(struct html *html)
{
	html_begintag(html, html_tags[SMALL]);
}

void
html_beginem(struct html *html)
{
	html_begintag(html, html_tags[EM]);
}

void
html_beginh1(struct html *html)
{
	html_begintag(html, html_tags[H1]);
}

void
html_beginh2(struct html *html)
{
	html_begintag(html, html_tags[H2]);
}

void
html_beginh3(struct html *html)
{
	html_begintag(html, html_tags[H3]);
}

void
html_beginh4(struct html *html)
{
	html_begintag(html, html_tags[H4]);
}

void
html_beginh5(struct html *html)
{
	html_begintag(html, html_tags[H5]);
}

void
html_beginh6(struct html *html)
{
	html_begintag(html, html_tags[H6]);
}

void
html_end(struct html *html)
{
	html_endtag(html, html_tags[HTML]);
}

void
html_endtag(struct html *html, const char *str)
{
	if (html->state == TAG)
		html_writec('>');

	html_writec('<');
	html_writec('/');
	html_write(str);
	html_writec('>');
	html->state = BEGIN;
}

void
html_endselftag(struct html *html)
{
	html_writec('>');
	html->state = BEGIN;
}

void
html_endattr(struct html *html)
{
	html_writec('"');
	html->state = TAG;
}

void
html_endhead(struct html *html)
{
	html_endtag(html, html_tags[HEAD]);
}

void
html_enddiv(struct html *html)
{
	html_endtag(html, html_tags[DIV]);
}

void
html_endmain(struct html *html)
{
	html_endtag(html, html_tags[MAIN]);
}

void
html_endarticle(struct html *html)
{
	html_endtag(html, html_tags[ARTICLE]);
}

void
html_endul(struct html *html)
{
	html_endtag(html, html_tags[UL]);
}

void
html_endol(struct html *html)
{
	html_endtag(html, html_tags[OL]);
}

void
html_endli(struct html *html)
{
	html_endtag(html, html_tags[LI]);
}


void
html_endp(struct html *html)
{
	html_endtag(html, html_tags[P]);
}

void
html_endtitle(struct html *html)
{
	html_endtag(html, html_tags[TITLE]);
}

void
html_endbody(struct html *html)
{
	html_endtag(html, html_tags[BODY]);
}

void
html_endmeta(struct html *html)
{
	html_endselftag(html);
}

void
html_endclass(struct html *html)
{
	html_endattr(html);
}

void
html_enddata(struct html *html)
{
	html_endattr(html);
}

void
html_endcharset(struct html *html)
{
	html_endattr(html);
}

void
html_endcontent(struct html *html)
{
	html_endattr(html);
}

void
html_endname(struct html *html)
{
	html_endattr(html);
}

void
html_endproperty(struct html *html)
{
	html_endattr(html);
}

void
html_endlink(struct html *html)
{
	html_endselftag(html);
}

void
html_endrel(struct html *html)
{
	html_endattr(html);
}

void
html_endlang(struct html *html)
{
	html_endattr(html);
}

void
html_endhref(struct html *html)
{
	html_endattr(html);
}

void
html_endtype(struct html *html)
{
	html_endattr(html);
}

void
html_endas(struct html *html)
{
	html_endattr(html);
}

void
html_endheader(struct html *html)
{
	html_endtag(html, html_tags[HEADER]);
}

void
html_endnav(struct html *html)
{
	html_endtag(html, html_tags[NAV]);
}

void
html_endanchor(struct html *html)
{
	html_endtag(html, html_tags[ANCHOR]);
}

void
html_endcode(struct html *html)
{
	html_endtag(html, html_tags[CODE]);
}

void
html_endimg(struct html *html)
{
	html_endselftag(html);
}

void
html_endheight(struct html *html)
{
	html_endattr(html);
}

void
html_endwidth(struct html *html)
{
	html_endattr(html);
}

void
html_endsrc(struct html *html)
{
	html_endattr(html);
}

void
html_endalt(struct html *html)
{
	html_endattr(html);
}

void
html_endsection(struct html *html)
{
	html_endtag(html, html_tags[SECTION]);
}

void
html_endtarget(struct html *html)
{
	html_endattr(html);
}

void
html_endcolor(struct html *html)
{
	html_endattr(html);
}

void
html_endspan(struct html *html)
{
	html_endtag(html, html_tags[SPAN]);
}

void
html_endfooter(struct html *html)
{
	html_endtag(html, html_tags[FOOTER]);
}

void
html_endsmall(struct html *html)
{
	html_endtag(html, html_tags[SMALL]);
}

void
html_endem(struct html *html)
{
	html_endtag(html, html_tags[EM]);
}

void
html_endh1(struct html *html)
{
	html_endtag(html, html_tags[H1]);
}

void
html_endh2(struct html *html)
{
	html_endtag(html, html_tags[H2]);
}

void
html_endh3(struct html *html)
{
	html_endtag(html, html_tags[H3]);
}

void
html_endh4(struct html *html)
{
	html_endtag(html, html_tags[H4]);
}

void
html_endh5(struct html *html)
{
	html_endtag(html, html_tags[H5]);
}

void
html_endh6(struct html *html)
{
	html_endtag(html, html_tags[H6]);
}

void
html_text(struct html *html, const char *format, ...)
{
	va_list ap;

	switch (html->state) {
	case TAG:
		html_writec('>');
		html->state = TEXT;
	break;
	case ATTRKEY:
		html->state = ATTRVAL;
	break;
	case ATTRVAL:
		html_writec(' ');
	break;
	default: ;
		/* Unrecheable */
	}

	va_start(ap, format);
	html_writef(format, ap);
	va_end(ap);
}

/*
 * TODO: sanitize text
 */

#endif /* HTML_IMPLEMENTATION */
