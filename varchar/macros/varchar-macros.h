#ifndef VARCHAR_MACROS_H
#define VARCHAR_MACROS_H

#include <string.h>
#include <ctype.h>

/*
 * Notes:
 * - These macros assume `v` is a `VARCHAR` struct (not pointer).
 * - `v.arr` must be a fixed-size array for `sizeof(v.arr)` to work.
 * - Maximum usable length is `sizeof(v.arr)`, but v.len can be less.
 */

/* Clear to empty */
#define VCLR(v)          do { (v).len = 0; memset((v).arr, 0, sizeof((v).arr)); } while(0)

/* Set from C string (truncate if needed) */
#define VSET(v, cstr)    do { \
    size_t __n = strlen(cstr); \
    if (__n > sizeof((v).arr)) __n = sizeof((v).arr); \
    memcpy((v).arr, (cstr), __n); \
    (v).len = (unsigned short)__n; \
} while(0)

/* Null-terminate into a buffer */
#define VTOC(v, buf)     do { \
    size_t __n = ((v).len < sizeof(buf)-1) ? (v).len : (sizeof(buf)-1); \
    memcpy((buf), (v).arr, __n); \
    (buf)[__n] = '\0'; \
} while(0)

/* Get length */
#define VLEN(v)          ((v).len)

/* Append another VARCHAR (truncate if needed) */
#define VAPPEND(v1, v2)  do { \
    size_t __max = sizeof((v1).arr); \
    size_t __free = (__max > (v1).len) ? (__max - (v1).len) : 0; \
    size_t __n = ((v2).len < __free) ? (v2).len : __free; \
    memcpy((v1).arr + (v1).len, (v2).arr, __n); \
    (v1).len += (unsigned short)__n; \
} while(0)

/* Compare two VARCHARs (case sensitive) */
#define VCMP(v1, v2)     (memcmp((v1).arr, (v2).arr, ((v1).len < (v2).len ? (v1).len : (v2).len)) ?: ((int)(v1).len - (int)(v2).len))

/* Compare two VARCHARs (case insensitive) */
#define VICMP(v1, v2)    varchar_icmp(&(v1), &(v2))

/* Uppercase in-place */
#define VUPPER(v)        do { \
    for (size_t __i = 0; __i < (v).len; __i++) (v).arr[__i] = (unsigned char)toupper((v).arr[__i]); \
} while(0)

/* Lowercase in-place */
#define VLOWER(v)        do { \
    for (size_t __i = 0; __i < (v).len; __i++) (v).arr[__i] = (unsigned char)tolower((v).arr[__i]); \
} while(0)

/* Trim both ends in-place */
#define VTRIM(v)         do { \
    int __s = 0, __e = (v).len - 1; \
    while (__s <= __e && isspace((unsigned char)(v).arr[__s])) __s++; \
    while (__e >= __s && isspace((unsigned char)(v).arr[__e])) __e--; \
    size_t __n = (__e >= __s) ? (__e - __s + 1) : 0; \
    if (__s > 0 && __n > 0) memmove((v).arr, (v).arr + __s, __n); \
    (v).len = (unsigned short)__n; \
} while(0)

/* Set to single character, repeated n times (or fill max) */
#define VFILL(v, ch, n)  do { \
    size_t __n = (n > sizeof((v).arr)) ? sizeof((v).arr) : n; \
    memset((v).arr, (ch), __n); \
    (v).len = (unsigned short)__n; \
} while(0)

/* Safe copy from one VARCHAR to another */
#define VCOPY(dest, src) do { \
    size_t __n = ((src).len < sizeof((dest).arr)) ? (src).len : sizeof((dest).arr); \
    memcpy((dest).arr, (src).arr, __n); \
    (dest).len = (unsigned short)__n; \
} while(0)

/* Zero memory but leave length intact */
#define VZERO(v)         memset((v).arr, 0, sizeof((v).arr))

#endif /* VARCHAR_MACROS_H */
