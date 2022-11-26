#include "utf_string.h"

int main()
{
    UtfString_t *_s = string_from_cstring("ε 😀 \xc0");
    printf("len: %ld code: %s\n", _s->length, string_charAt(_s, 2));

    string_free(_s);
    
    return 0;
}

