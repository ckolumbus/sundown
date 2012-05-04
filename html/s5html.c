/*
 * Copyright (c) 2009, Natacha Porté
 * Copyright (c) 2011, Vicent Marti
 * Copyright (c) 2012, Chris Drexler
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "markdown.h"
#include "s5html.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define USE_XHTML(opt) (opt->flags & HTML_USE_XHTML)


const char* s5header = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \n\
    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
\n\
<head>\n\
<title>S5: An Introduction</title>\n\
<!-- metadata -->\n\
<meta name=\"generator\" content=\"sundowns5\" />\n\
<meta name=\"author\" content=\"%s\" />\n\
<meta name=\"company\" content=\"%s\" />\n\
<meta name=\"presdate\" content=\"%s\" />\n\
<!-- configuration parameters -->\n\
<meta name=\"defaultView\" content=\"slideshow\" />\n\
<meta name=\"controlVis\" content=\"hidden\" />\n\
<!-- style sheet links -->\n\
<link rel=\"stylesheet\" href=\"ui/default/slides.css\" type=\"text/css\" media=\"projection\" id=\"slideProj\" />\n\
<link rel=\"stylesheet\" href=\"ui/default/outline.css\" type=\"text/css\" media=\"screen\" id=\"outlineStyle\" />\n\
<link rel=\"stylesheet\" href=\"ui/default/print.css\" type=\"text/css\" media=\"print\" id=\"slidePrint\" />\n\
<link rel=\"stylesheet\" href=\"ui/default/opera.css\" type=\"text/css\" media=\"projection\" id=\"operaFix\" />\n\
<!-- S5 JS -->\n\
<script src=\"ui/default/slides.js\" type=\"text/javascript\"></script>\n\
</head>\n\
<body>\n\
\n\
<div class=\"layout\">\n\
<div id=\"controls\"><!-- DO NOT EDIT --></div>\n\
<div id=\"currentSlide\"><!-- DO NOT EDIT --></div>\n\
<div id=\"header\"></div>\n\
<div id=\"footer\">\n\
<h1>%s</h1>\n\
<h2>%s &#8226; %s</h2>\n\
</div>\n\
\n\
</div>\n\
\n\
\n\
<div class=\"presentation\">\n";

const char* s5titleslide = "\
<div class=\"slide\"> \n \
<h1>%s</h1> \n \
<h2>%s</h2> \n \
<h3>%s</h3> \n \
<h4>%s</h4> \n \
</div>\n";


const char* s5footer=" \
</div> \n\
</body> \n\
</html> \n";



static void
s5_header(struct buf *ob, const struct buf *text, int level, void *opaque)
{
        struct html_s5_renderopt *options = (struct html_s5_renderopt*) opaque;

        if (level == 1){
            if (options->slideNr > 0) {
                    bufputs(ob, "</div>\n");
            }
            bufputs(ob, "<div class=\"slide\">\n");
            options->slideNr += 1;
        }

        if (ob->size)
            bufputc(ob, '\n');

        if (options->html_options.flags & HTML_TOC)
            bufprintf(ob, "<h%d id=\"toc_%d\">", level, options->html_options.toc_data.header_count++);
        else
            bufprintf(ob, "<h%d>", level);

        if (text) bufput(ob, text->data, text->size);
        bufprintf(ob, "</h%d>\n", level);
}


static void
s5_hrule(struct buf *ob, void *opaque)
{
        struct html_s5_renderopt *options = (struct html_s5_renderopt *) opaque;
        if (ob->size) bufputc(ob, '\n');

        if (options->slideNr > 0) {
                bufputs(ob, "</div>\n");
        }
        bufputs(ob, "<div class=\"slide\">\n");
        options->slideNr += 1;
}

static void
s5_docheader(struct buf *ob, struct mkd_fileheader *filehdr, void *opaque)
{
        extern const char* s5header;
        //struct html_renderopt *options = opaque;
        if (filehdr == NULL)
        {
        bufprintf(ob, s5header, "","","","","", "");
            bufprintf(ob, s5titleslide, "","","", "");
        } else 
        {
            const char * title    = bufcstr(filehdr->title);
            const char * subtitle = "";
            const char * author   = bufcstr(filehdr->author);
            const char * company  = "";
            const char * date     = bufcstr(filehdr->date);

            bufprintf(ob, s5header, author, company, date, title, author, date);
            bufprintf(ob, s5titleslide, title, subtitle, author, company);
        }
}

static void
s5_docfooter(struct buf *ob, void *opaque)
{
        struct html_s5_renderopt *options = (struct html_s5_renderopt*) opaque;
        if (options->slideNr > 0) {
                bufputs(ob, "</div>\n");
        }
        bufputs(ob, s5footer );
}


void
sdhtml_s5_renderer(struct sd_callbacks *callbacks, struct html_s5_renderopt *options, unsigned int render_flags)
{
        sdhtml_renderer(callbacks, (struct html_renderopt*)options, render_flags);
        callbacks->header     = s5_header;
        callbacks->hrule      = s5_hrule;
        callbacks->doc_header  = s5_docheader;
        callbacks->doc_footer  = s5_docfooter;
        options->slideNr = 0;
}
