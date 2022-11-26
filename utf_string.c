/**
 * Utf String 
 * Author: andydevs69420
 * Git: https://github.com/andydevs69420/utf-string.git
 * copyright © andydevs69420 2022
 **/

#include "utf_string.h"

static
int get_head_size(uchar _chr)
{
    if ((_chr & BIT_4) == BIT_4)
        return 4;
    if ((_chr & BIT_3) == BIT_3)
        return 3;
    if ((_chr & BIT_2) == BIT_2)
        return 2;
    if ((_chr & BIT_1) == 0x000)
        return 1;
    // 
    return 0;
}

static
int get_utf_size(codepoint _chr)
{
    if (_chr < 0x000080)
        return 1;
    if (_chr < 0x000800)
        return 2;
    if (_chr < 0x010000)
        return 3;
    if (_chr < 0x10FFFF)
        return 4;
    return 0;
}

/**
 * Creates utf string from char pointer.
 * @param char *_str
 * @returns UtfString_t
 **/
UtfString_t *string_from_cstring(char *_str)
{   UtfString_t *_string = 
        (UtfString_t*) malloc(sizeof(UtfString_t));

    _string->string = 
        (codepoint*) malloc(sizeof(codepoint));
    
    int _index = 0; codepoint _uc;

    for (int _iter = 0; _index < strlen(_str); _iter++)
    {
        int _size = get_head_size(_str[_index]);

        switch(_size)
        {
            case 1:
                _uc = _str[_index];
                _string->length++;
                break;
            
            case 2:
                _uc  = ((_str[ _index ] & FOLLOW_BIT_2) << 6); // xxx00000
                _uc |= ((_str[_index+1] & MAX_TRAIL_BIT));     // xx000000

                _string->length++;
                break;
            
            case 3:
                _uc  = ((_str[ _index ] & FOLLOW_BIT_3 ) << 12); // xxxx0000
                _uc |= ((_str[_index+1] & MAX_TRAIL_BIT) << 6);  // xx000000
                _uc |= ((_str[_index+2] & MAX_TRAIL_BIT));       // xx000000

                _string->length++;
                break;
        
            case 4:
                _uc  = ((_str[ _index ] & FOLLOW_BIT_4 ) << 18); // xxxxx000
                _uc |= ((_str[_index+1] & MAX_TRAIL_BIT) << 12); // xx000000
                _uc |= ((_str[_index+2] & MAX_TRAIL_BIT) <<  6); // xx000000
                _uc |= ((_str[_index+3] & MAX_TRAIL_BIT));       // xx000000

                _string->length++;
                break;

            default:
                fprintf(stderr, "[UtfStringError] bad utf-8 size %d.\n", _size);
                exit(1);
        }
        
        _index += _size; _string->string[_iter] = _uc;
        
        if (_index < strlen(_str)) {
            _string->string = realloc(_string->string, sizeof(codepoint) * (1 + (_iter + 1)));
        }
    }

    return _string;
}

/**
 * Returns the string representation(whole byte sequence) of unicode.
 * @param UtfString_t *_str
 * @param size_t _index
 * @returns char*
 **/
char *string_charAt(UtfString_t *_str, size_t _index)
{   
    codepoint _uc = string_charCodePointAt(_str, _index);

    int _count  = 0;
    uchar *_chr =
        (char*) malloc(sizeof(char) + 1); _chr[0] = '\0';

#ifdef UTF_STRING_H
#define pushChar(c) { \
    _chr[_count  ] = (uchar) c; \
    _chr[_count+1] = '\0'; \
    _chr = (char*) realloc(_chr, sizeof(char) * ++_count); \
}
#endif

    switch (get_utf_size(_uc))
    {
        case 1:
            pushChar(_uc);
            break;
        case 2:
            pushChar((_uc >> 6) | BIT_2);
            pushChar((_uc & MAX_TRAIL_BIT) | TRAIL);
            break;
        case 3:
            pushChar((_uc >> 12) | BIT_3);
            pushChar(((_uc >> 6) & MAX_TRAIL_BIT) | TRAIL);
            pushChar((_uc & MAX_TRAIL_BIT) | TRAIL);
            break;
        case 4:
            pushChar(( _uc >> 18) | BIT_4);
            pushChar(((_uc >> 12) & MAX_TRAIL_BIT) | TRAIL);
            pushChar(((_uc >>  6) & MAX_TRAIL_BIT) | TRAIL);
            pushChar(( _uc  & MAX_TRAIL_BIT) | TRAIL);
            break;
    }
    return _chr;
}

/**
 * Returns the codepoint from the given index.
 * Note: String index starts at zero(0).
 * @param UtfString_t *_str
 * @param size_t _index
 * @returns codepoint
 **/
codepoint string_charCodePointAt(UtfString_t *_str, size_t _index)
{ 
    if (_str->length <= 0 || !(_index >= 0 && _index < _str->length))
    {   fprintf(stderr, "[UtfStringError] index out of bounds 0~%ld, got %ld.\n", _str->length, _index);
        exit(1);
    }

    return _str->string[_index];
}

/**
 * Returns the length of the string.
 * @param UtfString_t *_str
 * @returns size_t
 **/
size_t string_length(UtfString_t *_str)
{ return _str->length; }

/**
 * Deallocate UtfString_t
 * @param UtfString_t *_str
 * @returns void
 **/
void string_free(UtfString_t *_str)
{   _str->length = 0;
    free(_str->string);
    free(_str);
}

