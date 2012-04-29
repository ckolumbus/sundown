#ifndef UPSKIRT_S5HTML_H
#define UPSKIRT_S5HTML_H

#include "html.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct html_s5_renderopt {
   struct html_renderopt html_options;
   int slideNr;
   int insideNotes;
};

extern void
sdhtml_s5_renderer(struct sd_callbacks *callbacks, struct html_s5_renderopt *options_ptr, unsigned int render_flags);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif

#endif

