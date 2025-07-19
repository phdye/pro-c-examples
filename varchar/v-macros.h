#ifndef V_MACROS_H
#define V_MACROS_H

#include <string.h>
#include <ctype.h>
#include <stddef.h>

/* Utility macros for working with Oracle VARCHAR structs. */

/* Determine capacity of VARCHAR .arr */
#define VCAP(v) (sizeof((v).arr))

/* Convert C string to VARCHAR */
#define V_FROM_CSTR(v, s) \
    do { \
        size_t __n = strlen(s); \
        if (__n > VCAP(v)) __n = VCAP(v); \
        memcpy((v).arr, (s), __n); \
        (v).len = (unsigned short)__n; \
    } while (0)

/* Convert n characters of C string to VARCHAR */
#define V_FROM_CSTR_N(v, s, n) \
    do { \
        size_t __n = (n); \
        if (__n > VCAP(v)) __n = VCAP(v); \
        memcpy((v).arr, (s), __n); \
        (v).len = (unsigned short)__n; \
    } while (0)

/* Copy VARCHAR to C string buffer (buffer must be char array) */
#define V_TO_CSTR(buf, v) \
    do { \
        size_t __n = (v).len; \
        if (__n >= sizeof(buf)) __n = sizeof(buf) - 1; \
        memcpy((buf), (v).arr, __n); \
        (buf)[__n] = '\0'; \
    } while (0)

/* Copy one VARCHAR to another */
#define V_COPY(dst, src) \
    do { \
        size_t __n = (src).len; \
        if (__n > VCAP(dst)) __n = VCAP(dst); \
        memcpy((dst).arr, (src).arr, __n); \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Length of a VARCHAR */
#define V_LEN(v) ((size_t)(v).len)

/* Case sensitive compare */
#define V_CMP(v1, v2) \ 
    ({ \
        size_t __l1 = (v1).len; \
        size_t __l2 = (v2).len; \
        size_t __m = (__l1 < __l2 ? __l1 : __l2); \
        int __r = memcmp((v1).arr, (v2).arr, __m); \
        (__r != 0) ? __r : (int)(__l1 - __l2); \
    })

/* Compare first n bytes */
#define V_NCMP(v1, v2, n) \ 
    ({ \
        size_t __n = (n); \
        size_t __l1 = ((v1).len < __n ? (v1).len : __n); \
        size_t __l2 = ((v2).len < __n ? (v2).len : __n); \
        int __r = memcmp((v1).arr, (v2).arr, (__l1 < __l2 ? __l1 : __l2)); \
        (__r != 0) ? __r : (int)(__l1 - __l2); \
    })

/* Case insensitive compare */
#define V_ICMP(v1, v2) \ \
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
#define V_NICMP(v1, v2, n) \ 
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

/* Concatenate two VARCHARs into dest */
#define V_CONCAT(dst, v1, v2) \
    do { \
        size_t __max = VCAP(dst); \
        size_t __l1 = (v1).len > __max ? __max : (v1).len; \
        size_t __l2 = (v2).len; \
        if (__l1 + __l2 > __max) __l2 = __max - __l1; \
        memcpy((dst).arr, (v1).arr, __l1); \
        memcpy((dst).arr + __l1, (v2).arr, __l2); \
        (dst).len = (unsigned short)(__l1 + __l2); \
    } while (0)

/* Concatenate up to n bytes from v2 */
#define V_CONCAT_N(dst, v1, v2, n) \
    do { \
        size_t __max = VCAP(dst); \
        size_t __l1 = (v1).len > __max ? __max : (v1).len; \
        size_t __l2 = (v2).len > (n) ? (n) : (v2).len; \
        if (__l1 + __l2 > __max) __l2 = __max - __l1; \
        memcpy((dst).arr, (v1).arr, __l1); \
        memcpy((dst).arr + __l1, (v2).arr, __l2); \
        (dst).len = (unsigned short)(__l1 + __l2); \
    } while (0)

/* Find index of char, -1 if not found */
#define V_INDEX_CHAR(v, ch) \ 
    ({ \
        int __idx = -1; \
        for (size_t __i = 0; __i < (v).len; ++__i) { \
            if ((v).arr[__i] == (ch)) { __idx = (int)__i; break; } \
        } \
        __idx; \
    })

/* Find substring index */
#define V_INDEX_OF(hay, needle) \ 
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
#define V_INDEX_OF_ANY(v, set) \ 
    ({ \
        int __idx = -1; \
        for (size_t __i = 0; __i < (v).len; ++__i) { \
            if (strchr((set), (v).arr[__i])) { __idx = (int)__i; break; } \
        } \
        __idx; \
    })

/* Span of characters from accept */
#define V_SPAN(v, accept) \ 
    ({ \
        size_t __i = 0; \
        while (__i < (v).len && strchr((accept), (v).arr[__i])) ++__i; \
        __i; \
    })

/* Span of characters not in reject */
#define V_CSPAN(v, reject) \ 
    ({ \
        size_t __i = 0; \
        while (__i < (v).len && !strchr((reject), (v).arr[__i])) ++__i; \
        __i; \
    })

/* Trim whitespace from both ends */
#define V_TRIM(dst, src) \
    do { \
        int __s = 0; \
        int __e = (int)(src).len - 1; \
        while (__s <= __e && isspace((unsigned char)(src).arr[__s])) ++__s; \
        while (__e >= __s && isspace((unsigned char)(src).arr[__e])) --__e; \
        size_t __n = (__e >= __s) ? (size_t)(__e - __s + 1) : 0; \
        if (__n > VCAP(dst)) __n = VCAP(dst); \
        memcpy((dst).arr, (src).arr + __s, __n); \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Uppercase conversion */
#define V_UPPER(dst, src) \
    do { \
        size_t __n = (src).len; \
        if (__n > VCAP(dst)) __n = VCAP(dst); \
        for (size_t __i = 0; __i < __n; ++__i) \
            (dst).arr[__i] = (unsigned char)toupper((unsigned char)(src).arr[__i]); \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Lowercase conversion */
#define V_LOWER(dst, src) \
    do { \
        size_t __n = (src).len; \
        if (__n > VCAP(dst)) __n = VCAP(dst); \
        for (size_t __i = 0; __i < __n; ++__i) \
            (dst).arr[__i] = (unsigned char)tolower((unsigned char)(src).arr[__i]); \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Move overlapping regions */
#define V_MOVE(dst, src) \
    do { \
        size_t __n = (src).len; \
        if (__n > VCAP(dst)) __n = VCAP(dst); \
        memmove((dst).arr, (src).arr, __n); \
        (dst).len = (unsigned short)__n; \
    } while (0)

/* Fill VARCHAR with ch */
#define V_SET(v, ch, n) \
    do { \
        size_t __n = (n); \
        if (__n > VCAP(v)) __n = VCAP(v); \
        memset((v).arr, (ch), __n); \
        (v).len = (unsigned short)__n; \
    } while (0)

/* Zero out VARCHAR */
#define V_BZERO(v) \
    do { \
        memset((v).arr, 0, VCAP(v)); \
        (v).len = 0; \
    } while (0)

#endif /* V_MACROS_H */
