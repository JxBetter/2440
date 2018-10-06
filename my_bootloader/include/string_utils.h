#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include "uart.h"


int get_int(void);
char *gets(char s[]);
unsigned int get_uint(void);
unsigned int strlen(const char * s);
int strcmp(const char * cs,const char * ct);
void strcpy(char *dest, char *src);
long simple_strtol(const char *cp,char **endp,unsigned int base);

#endif /* _STRING_UTILS_H */

