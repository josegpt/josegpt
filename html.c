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


#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "josegpt.c"

#define MAXAGE 60 * 60

static void html_header(const char *);
static void html_footer(void);
static void html_portfolio(void);

int
main(void)
{
	if (pledge("stdio", NULL) == -1)
		err(EXIT_FAILURE, "pledge");
	puts("Status: 200 OK\r");
	printf("Cache-Control: max-age=%d\r\n", MAXAGE);
	puts("Content-Type: text/html\r");
	puts("\r");
	html_header("Portfolio");
	html_portfolio();
	html_footer();
	return EXIT_SUCCESS;
}

static void
html_header(const char *title)
{
	puts("<!doctype html>");
	puts("<meta charset=\"utf-8\">");

	printf("<title>josegpt &sol; %s</title>\n", title);
	puts("<meta content=\"width=device-width, initial-scale=1\" name=\"viewport\">");
	puts("<meta content=\"josegpt\" name=\"author\">");
	puts("<meta content=\"software engineer\" name=\"description\">");
	puts("<meta content=\"HTML, CSS, JSON, C\" name=\"keywords\">");
	puts("<meta content=\"red\" name=\"theme-color\">");
	puts("<meta content=\"website\" property=\"og:type\">");
	printf("<meta content=\"josegpt &sol; %s\" property=\"og:title\">\n", title);
	puts("<meta content=\"https://josegpt.com\" property=\"og:url\">");
	puts("<meta content=\"software engineer\" property=\"og:description\">");
	puts("<meta content=\"/josegpt/banner.png\" property=\"og:image\">");
	puts("<link rel=\"icon\" href=\"/josegpt/favicon.ico\" type=\"image/x-icon\">");
	puts("<link rel=\"preload stylesheet\" type=\"text/css\" as=\"style\" href=\"/josegpt/style.css\">");

	puts("<body class=\"cover\">");
	puts("<header>");
	puts("<nav data-align=\"center\" class=\"cluster\">");
	puts("<a class=\"cluster\" href=\"/\">");
	puts("<img height=\"30\" width=\"60\" src=\"/josegpt/logo.svg\" alt=\"Logo\">");
	puts("</a>");

	puts("<div>");
	printf("<span>%s</span>", title);
	puts("</div>");

	puts("</nav>");

	puts("</header>");
}

static void
html_footer(void)
{
	time_t now;
	struct tm *datetime;

	time(&now);
	datetime = localtime(&now);
	puts("<nav data-justify=\"center\" class=\"cluster\">");
	puts("<a href=\"mailto:josegpt27@gmail.com\">Email</a>");
	puts("<a target=\"_blank\" rel=\"noopener noreferrer\" href=\"https://www.linkedin.com/in/josegpt\">Linkedin</a>");
	puts("<a target=\"_blank\" rel=\"noopener noreferrer\" href=\"https://sr.ht/~josegpt/\">Sourcehut</a>");
	puts("<a target=\"_blank\" rel=\"noopener noreferrer\" href=\"https://github.com/josegpt\">GitHub</a>");
	puts("<a target=\"_blank\" rel=\"noopener noreferrer\" href=\"https://codepen.io/josegpt\">Codepen</a>");
	puts("</nav>");

	puts("<footer data-justify=\"center\" data-align=\"center\" class=\"cluster\">");
	puts("<a href=\"/\" class=\"cluster\">");
	puts("<img height=\"15\" width=\"30\" src=\"/josegpt/logo.svg\" alt=\"Logo\">");
	puts("</a>");
	printf("<small>&copy; 2017 &mdash; %d <em>josegpt&commat;%.2f</em></small>\n", datetime->tm_year + 1900, VERSION);
	puts("</footer>");
}

static void
html_portfolio(void)
{
	puts("<main data-justify=\"center\" data-align=\"center\" class=\"cluster\">");
	puts("<section class=\"text-align:center\">");
	puts("<h1>No Projects Found</h1>");
	puts("</section>");
	puts("</main>");
}

