#ifndef __FILEUTIL_H__
#define __FILEUTIL_H__

int file_write(const char *fn, void *buf, size_t len);
int file_append(const char *fn, void *buf, size_t len);

#endif
