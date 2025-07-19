#ifndef VARCHAR_UTIL_H
#define VARCHAR_UTIL_H

#include <stddef.h>  // for size_t
#include <sqlca.h>   // for VARCHAR

#ifdef __cplusplus
extern "C" {
#endif

/* C string ↔ VARCHAR */
void cstr_to_varchar(const char *src, VARCHAR *v, size_t max_vlen);
void cstr_to_varchar_n(const char *src, size_t n, VARCHAR *v, size_t max_vlen);
void varchar_to_cstr(const VARCHAR *v, char *dest, size_t maxlen);

/* VARCHAR copy, clone, length */
void varchar_copy(const VARCHAR *src, VARCHAR *dest, size_t max_dest_len);
VARCHAR *varchar_strdup(const VARCHAR *src);
size_t varchar_len(const VARCHAR *v);

/* VARCHAR comparison */
int varchar_cmp(const VARCHAR *v1, const VARCHAR *v2);
int varchar_ncmp(const VARCHAR *v1, const VARCHAR *v2, size_t n);
int varchar_icmp(const VARCHAR *v1, const VARCHAR *v2);
int varchar_nicmp(const VARCHAR *v1, const VARCHAR *v2, size_t n);

/* VARCHAR concatenation */
void varchar_concat(const VARCHAR *v1, const VARCHAR *v2, VARCHAR *result, size_t max_result_len);
void varchar_concat_n(const VARCHAR *v1, const VARCHAR *v2, VARCHAR *result, size_t n, size_t max_result_len);

/* Character / substring search */
int varchar_index_char(const VARCHAR *v, char c);
int varchar_index_of(const VARCHAR *haystack, const VARCHAR *needle);
int varchar_index_of_any(const VARCHAR *v, const char *set);

/* Span functions */
size_t varchar_span(const VARCHAR *v, const char *accept);
size_t varchar_cspan(const VARCHAR *v, const char *reject);

/* Whitespace trimming */
void varchar_trim(const VARCHAR *src, VARCHAR *dest, size_t max_dest_len);

/* Case conversion */
void varchar_upper(const VARCHAR *src, VARCHAR *dest, size_t max_dest_len);
void varchar_lower(const VARCHAR *src, VARCHAR *dest, size_t max_dest_len);

/* Memory-like operations */
void varchar_move(VARCHAR *dest, const VARCHAR *src, size_t max_dest_len);
void varchar_set(VARCHAR *v, char ch, size_t len, size_t max_len);
void varchar_bzero(VARCHAR *v, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif // VARCHAR_UTIL_H
