#ifndef __TIMEUTIL_H__
#define __TIMEUITL_H__

/* timestamp in us */
long long get_timestamp(void);
void print_timestamp(const char *type, long long ts);

#endif
