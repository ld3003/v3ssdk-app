#ifndef __USBDBG_H__
#define __USBDBG_H__

#include "imutil.h"

int usbdbg_connect(void);
void usbdbg_close(void);
int usbdbg_send(const uint8_t *buf, int size);
int usbdbg_send_jpeg(const char *title, const char *jpeg_buffer, int w, int h, int size);
int usbdbg_send_image(const char *title, image_t *img, int quality);

#endif
