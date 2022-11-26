#include "utf_string.h"

int main()
{
    UtfString_t *_s = string_from_cstring("ε大😀ε大😀ε大😀ε大😀ε大😀ε大😀ε大😀ε大😀ε大😀ε大😀ε大");
    printf("len: %ld code: %s\n", _s->length, string_charAt(_s, string_length(_s)-1));

    string_free(_s);
    
    return 0;
}

