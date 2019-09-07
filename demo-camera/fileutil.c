#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fileutil.h"

int file_write(const char *fn, void *buf, size_t len)
{
	int ret;
	
	FILE *f = fopen(fn, "w");
	if (f) {
		ret = fwrite(buf, 1, len, f);
		fflush(f);
		fclose(f);
		return ret;
	}

	printf("[%s]\n", __FUNCTION__);
	printf("\tFailed!\n");
	return -1;
}

int file_append(const char *fn, void *buf, size_t len)
{
	int ret;
	
	FILE *f = fopen(fn, "a+");
	if (f) {
		ret = fwrite(buf, 1, len, f);
		fflush(f);
		fclose(f);
		return ret;
	}

	printf("[%s]\n", __FUNCTION__);
	printf("\tFailed!\n");
	return -1;
}
