// Macro to find the first null byte in a character array
// up to a specified maximum length.

#define FIND_FIRST_NUL_BYTE(arr, max_len)      \
    ({                                         \
        char * found = NULL;                   \
        for (size_t i = 0; i < max_len; ++i) { \
            if (arr[i] == '\0') {              \
                found = &arr[i];               \
                break;                         \
            }                                  \
        }                                      \
        ret;                                   \
    })

#define VARCHAR_INIT(v)                        \
    {                                          \
        (v).len = 0;                           \
        if (sizeof((v).arr) > 0) {             \
            memset((v).arr, 0x00, sizeof((v).arr)); \
        }                                      \
    }

#define VARCHAR_CHECK_LEN(v)                   \
    {                                          \
        if ((v).len >= sizeof((v).arr)) {      \
            fprintf(stderr, "Line %d, varchar %s : .len %d >= %lu sizeof(.arr)\n", \
                 __LINE__, #v, (v).len, sizeof((v).arr)); \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
    }

#define VARCHAR_CHECK_STR(v)                   \
    {                                          \
        unsigned siz = sizeof((v).arr);        \
        char *nul = FIND_FIRST_NULL_BYTES((v).arr, siz); \
        if (nul == NULL) {                     \
            fprintf(stderr, "Line %d : VARCHAR_CHECK_STR(%s) : No NUL byte found within %u sizeof(.arr) bytes\n", \
                 __LINE__, #v, siz);           \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
    }

#define VARCHAR_CHECK(v)                       \
    {                                          \
        VARCHAR_CHECK_LEN(v);                  \
        VARCHAR_CHECK_STR(v);                  \
    }

// Set the VARCHAR .len based on the length of the C string in .arr
// `v` must have just be written as a C string.
#define VARCHAR_ZSETLEN(v)                     \
    {                                          \
        unsigned len = strlen((v).arr);        \
        unsigned max = sizeof(v.arr);          \
        if (len >= max) {                      \
            fprintf(stderr, "Line %d : VARCHAR_ZSETLEN(%s) : strlen(.arr) %u >= %u sizeof(.arr)\n", \
                __LINE__, #v, len, max );      \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
        (v).len = len;                         \
    }

// Nul terminate the VARCHAR .arr to make it a C string.
// `v` came from the database and you wish to apply nul termination
#define VARCHAR_SETLENZ(v)                     \
    {                                          \
        unsigned max = sizeof((v).arr);        \
        if ((v).len >= max) {                  \
            fprintf(stderr, "Line %d : VARCHAR_SETLENZ(%s) : .len %u >= %u sizeof(.arr)\n", \
                __LINE__, #v, (v).len, max );  \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
        (v).arr[(v).len] = '\0';               \
    }

// Copy one VARCHAR to another VARCHAR
#define VARCHAR_COPY(dst, src)                 \
    {                                          \
        unsigned len = (src).len ;             \
        unsigned max = sizeof((dst).arr);      \
        if (len >= max) {                      \
            fprintf(stderr, "Line %d : VARCHAR_COPY %s <- %s : src.len %u >= %u sizeof(dst.arr)\n", \
                __LINE__, #dst, #src, src.len, max); \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
        memcpy((char*)dst.arr, (char*)src.arr, len); \
        (dst).len = (src).len;                 \
        (dst).arr[(dst).len] = '\0';           \
    }

// Copy a C String into a VARCHAR
#define VARCHAR_COPY_IN(v, src)                \
    {                                          \
        unsigned len = strlen(src);            \
        unsigned siz = sizeof((v).arr);        \
        if (len >= siz) {                      \
            fprintf(stderr, "Line %d, VARCHAR_COPY_IN %s <- %s : sizeof(dst.arr) %u <= %u strlen(src)\n", \
               __LINE__, #v, #src, siz, len);  \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
        strcpy((char*)(v).arr, (src));         \
        VARCHAR_ZSETLEN(v);                    \
    }

// Copy a VARCHAR out to a C String
// Note: dst must be a character array, not a character pointer
#define VARCHAR_COPY_OUT(dst, v)               \
    {                                          \
        VARCHAR_CHECK_LEN(v);                  \
        unsigned len = (v).len;                \
        unsigned siz = sizeof(dst);            \
        if (len >= siz) {                      \
            fprintf(stderr, "Line %d, VARCHAR_COPY_OUT %s <- %s : sizeof(dst) %u <= %u src.len\n", \
                __LINE__, #dst, #v, siz, len); \
            { int *p =NULL; *p = 0; }          \
            abort();                           \
        }                                      \
        memcpy((char*)(dst), (char*)(v).arr, len); \
        (dst)[len] = '\0';                     \
    }
