#pragma once

#define memset(d, v, l)         __builtin_memset(d, v, l)
#define memcpy(d, s, l)         __builtin_memcpy(d, s, l)
#define memcmp(a, b, l)         __builtin_memcmp(a, b, l)

#define strcat(d, s)            __builtin_strcat(d, s)
#define strcpy(d, s)            __builtin_strcpy(d, s)
#define strncpy(d, s, l)        __builtin_strncpy(d, s, l)
#define strcmp(a, b)            __builtin_strcmp(a, b)
#define strncmp(a, b, l)        __builtin_strncmp(a, b, l)
#define strlen(a)               __builtin_strlen(a)
#define strstr(str1, str2)      __builtin_strstr(str1, str2)
