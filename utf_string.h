

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "utf_data.h"

#ifndef UTF_STRING_H
#define UTF_STRING_H


#ifdef __cplusplus
extern "C" {
#endif


enum {
    BIT_1 = 0x80,        // 00000000
    BIT_2 = 0xc0,        // 11000000
    BIT_3 = 0xe0,        // 11100000
    BIT_4 = 0xf0,        // 11110000

    FOLLOW_BIT_2 = 0x1f, // 00011111
    FOLLOW_BIT_3 = 0x0f, // 00001111
    FOLLOW_BIT_4 = 0x07, // 00000111

    TRAIL = BIT_1,       // 10000000
    MAX_TRAIL_BIT = 0x3f // xx111111
};


typedef unsigned int codepoint;
typedef unsigned char uchar;


typedef struct utf_string 
{
    codepoint *string;
    size_t length;
} UtfString_t;


UtfString_t *string_from_cstring(char*);
char *string_charAt(UtfString_t *, size_t);
codepoint string_charCodePointAt(UtfString_t*, size_t);
size_t string_length(UtfString_t*);
void string_free(UtfString_t*);

#ifdef __cplusplus
}
#endif

#endif
