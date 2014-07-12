#ifndef __XIF_H

#include <xcb/xcb_image.h>

void xif_write(xcb_image_t *, char *);
xcb_image_t *xif_read(char *);

#define __XIF_H 1
#endif
