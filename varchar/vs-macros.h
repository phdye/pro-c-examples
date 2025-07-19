#ifndef VS_MACROS_H
#define VS_MACROS_H

#include <string.h>
#include <ctype.h>
#include <stddef.h>

/* Utility macros for VARCHAR structs that always NUL terminate results. */

/* Determine usable capacity excluding space for terminating NUL */
#define VS_CAP(v) (sizeof((v).arr) > 0 ? sizeof((v).arr) - 1 : 0)

/* Convert C string to VARCHAR with NUL termination */
#define VS_FROM_CSTR(v, s) \
    do { \
        size_t __cap = VS_CAP(v); \
        size_t __n = strlen(s); \
        if (__n > __cap) __n = __cap; \
        memcpy((v).arr, (s), __n); \
        (v).arr[__n] = '\0'; \
        (v).len = (unsigned short)__n; \
    } while (0)

/* Convert n characters of C string to VARCHAR with NUL termination */
#define VS_FROM_CSTR_N(v, s, n) \
    do { \
        size_t __cap = VS_CAP(v); \
        size_t __n = (n); \
        if (__n > __cap) __n = __cap; \
        memcpy((v).arr, (s), __n); \
        (v).arr[__n] = '\0'; \
        (v).len = (unsigned short)__n; \
    } while (0)

/* Copy VARCHAR to C string buffer (buffer must be char array) */
#define VS_TO_CSTR(buf, v) \
    do { \
        size_t __n = (v).len; \
        if (__n >= sizeof(buf)) __n = sizeof(buf) - 1; \
        memcpy((buf), (v).arr, __n); \
        (buf)[__n] = '\0'; \
    } while (0)

/* Copy one VARCHAR to another with NUL termination */
#define VS_COPY(dst, src) \
    do { \
        size_t __cap = VS_CAP(dst); \
        size_t __n = (src).len; \
        if (__n > __cap) __n = __cap; \
        memcpy((dst).arr, (src).arr, __n); \
        (dst).arr[__n] = '\0'; \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Length of a VARCHAR */
#define VS_LEN(v) ((size_t)(v).len)

/* Case sensitive compare */
#define VS_CMP(v1, v2) \
    ({ \
        size_t __l1 = (v1).len; \
        size_t __l2 = (v2).len; \
        size_t __m = (__l1 < __l2 ? __l1 : __l2); \
        int __r = memcmp((v1).arr, (v2).arr, __m); \
        (__r != 0) ? __r : (int)(__l1 - __l2); \
    })

/* Compare first n bytes */
#define VS_NCMP(v1, v2, n) \
    ({ \
        size_t __n = (n); \
        size_t __l1 = ((v1).len < __n ? (v1).len : __n); \
        size_t __l2 = ((v2).len < __n ? (v2).len : __n); \
        int __r = memcmp((v1).arr, (v2).arr, (__l1 < __l2 ? __l1 : __l2)); \
        (__r != 0) ? __r : (int)(__l1 - __l2); \
    })

/* Case insensitive compare */
#define VS_ICMP(v1, v2) \
    ({ \
        size_t __l1 = (v1).len; \
        size_t __l2 = (v2).len; \
        size_t __m = (__l1 < __l2 ? __l1 : __l2); \
        int __r = 0; \
        for (size_t __i = 0; __i < __m; ++__i) { \
            int __c1 = tolower((unsigned char)(v1).arr[__i]); \
            int __c2 = tolower((unsigned char)(v2).arr[__i]); \
            if (__c1 != __c2) { __r = __c1 - __c2; break; } \
        } \
        (__r != 0) ? __r : (int)(__l1 - __l2); \
    })

/* Case insensitive n compare */
#define VS_NICMP(v1, v2, n) \
    ({ \
        size_t __n = (n); \
        size_t __l1 = ((v1).len < __n ? (v1).len : __n); \
        size_t __l2 = ((v2).len < __n ? (v2).len : __n); \
        int __r = 0; \
        for (size_t __i = 0; __i < (__l1 < __l2 ? __l1 : __l2); ++__i) { \
            int __c1 = tolower((unsigned char)(v1).arr[__i]); \
            int __c2 = tolower((unsigned char)(v2).arr[__i]); \
            if (__c1 != __c2) { __r = __c1 - __c2; break; } \
        } \
        (__r != 0) ? __r : (int)(__l1 - __l2); \
    })

/* Concatenate two VARCHARs into dest with NUL termination */
#define VS_CONCAT(dst, v1, v2) \
    do { \
        size_t __cap = VS_CAP(dst); \
        size_t __l1 = (v1).len > __cap ? __cap : (v1).len; \
        size_t __l2 = (v2).len; \
        if (__l1 + __l2 > __cap) __l2 = __cap - __l1; \
        memcpy((dst).arr, (v1).arr, __l1); \
        memcpy((dst).arr + __l1, (v2).arr, __l2); \
        (dst).arr[__l1 + __l2] = '\0'; \
        (dst).len = (unsigned short)(__l1 + __l2); \
    } while (0)

/* Concatenate up to n bytes from v2 with NUL termination */
#define VS_CONCAT_N(dst, v1, v2, n) \
    do { \
        size_t __cap = VS_CAP(dst); \
        size_t __l1 = (v1).len > __cap ? __cap : (v1).len; \
        size_t __l2 = (v2).len > (n) ? (n) : (v2).len; \
        if (__l1 + __l2 > __cap) __l2 = __cap - __l1; \
        memcpy((dst).arr, (v1).arr, __l1); \
        memcpy((dst).arr + __l1, (v2).arr, __l2); \
        (dst).arr[__l1 + __l2] = '\0'; \
        (dst).len = (unsigned short)(__l1 + __l2); \
    } while (0)

/* Find index of char, -1 if not found */
#define VS_INDEX_CHAR(v, ch) \
    ({ \
        int __idx = -1; \
        for (size_t __i = 0; __i < (v).len; ++__i) { \
            if ((v).arr[__i] == (ch)) { __idx = (int)__i; break; } \
        } \
        __idx; \
    })

/* Find substring index */
#define VS_INDEX_OF(hay, needle) \
    ({ \
        int __idx = -1; \
        if ((needle).len <= (hay).len) { \
            for (size_t __i = 0; __i <= (size_t)((hay).len - (needle).len); ++__i) { \
                if (memcmp((hay).arr + __i, (needle).arr, (needle).len) == 0) { \
                    __idx = (int)__i; break; \
                } \
            } \
        } \
        __idx; \
    })

/* Find first occurrence of any char from set */
#define VS_INDEX_OF_ANY(v, set) \
    ({ \
        int __idx = -1; \
        for (size_t __i = 0; __i < (v).len; ++__i) { \
            if (strchr((set), (v).arr[__i])) { __idx = (int)__i; break; } \
        } \
        __idx; \
    })

/* Span of characters from accept */
#define VS_SPAN(v, accept) \
    ({ \
        size_t __i = 0; \
        while (__i < (v).len && strchr((accept), (v).arr[__i])) ++__i; \
        __i; \
    })

/* Span of characters not in reject */
#define VS_CSPAN(v, reject) \
    ({ \
        size_t __i = 0; \
        while (__i < (v).len && !strchr((reject), (v).arr[__i])) ++__i; \
        __i; \
    })

/* Trim whitespace from both ends with NUL termination */
#define VS_TRIM(dst, src) \
    do { \
        int __s = 0; \
        int __e = (int)(src).len - 1; \
        while (__s <= __e && isspace((unsigned char)(src).arr[__s])) ++__s; \
        while (__e >= __s && isspace((unsigned char)(src).arr[__e])) --__e; \
        size_t __cap = VS_CAP(dst); \
        size_t __n = (__e >= __s) ? (size_t)(__e - __s + 1) : 0; \
        if (__n > __cap) __n = __cap; \
        memcpy((dst).arr, (src).arr + __s, __n); \
        (dst).arr[__n] = '\0'; \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Uppercase conversion with NUL termination */
#define VS_UPPER(dst, src) \
    do { \
        size_t __cap = VS_CAP(dst); \
        size_t __n = (src).len; \
        if (__n > __cap) __n = __cap; \
        for (size_t __i = 0; __i < __n; ++__i) \
            (dst).arr[__i] = (unsigned char)toupper((unsigned char)(src).arr[__i]); \
        (dst).arr[__n] = '\0'; \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Lowercase conversion with NUL termination */
#define VS_LOWER(dst, src) \
    do { \
        size_t __cap = VS_CAP(dst); \
        size_t __n = (src).len; \
        if (__n > __cap) __n = __cap; \
        for (size_t __i = 0; __i < __n; ++__i) \
            (dst).arr[__i] = (unsigned char)tolower((unsigned char)(src).arr[__i]); \
        (dst).arr[__n] = '\0'; \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Move overlapping regions with NUL termination */
#define VS_MOVE(dst, src) \
    do { \
        size_t __cap = VS_CAP(dst); \
        size_t __n = (src).len; \
        if (__n > __cap) __n = __cap; \
        memmove((dst).arr, (src).arr, __n); \
        (dst).arr[__n] = '\0'; \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Fill VARCHAR with ch and NUL terminate */
#define VS_SET(v, ch, n) \
    do { \
        size_t __cap = VS_CAP(v); \
        size_t __n = (n); \
        if (__n > __cap) __n = __cap; \
        memset((v).arr, (ch), __n); \
        (v).arr[__n] = '\0'; \
        (v).len = (unsigned short)__n; \
    } while (0)

/* Zero out VARCHAR and NUL terminate */
#define VS_BZERO(v) \
    do { \
        memset((v).arr, 0, sizeof((v).arr)); \
        (v).len = 0; \
    } while (0)

#endif /* VS_MACROS_H */
