#include "inc/main.h"

unsigned int string_count(const char *str, const char *to) {
    unsigned int i,count;
    for(i=0,count=0; i<strlen(str)-strlen(to); i++)
        if(!memcmp(str+i, to, strlen(to))) count++;
    return count;
}

void string_replace(char *dest, const char *src, const char *to, const char *with) {
    unsigned int i,c;
    for(i=0,c=0; i < strlen(src); i++,c++) {
        if(memcmp(src+i, to, strlen(to))) {
            dest[c] = src[i];
            continue;
        }
        memcpy(dest+c, with, strlen(with));
        c+=strlen(with)-1;
        i+=strlen(to)  -1;
    }
    dest[c] = 0;
}
