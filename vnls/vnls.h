/*
Macros:
    VNSL_NO_GENERICS

*/


#ifndef VNLS_H
#define VNLS_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>



typedef struct VnlS_String {
    const char *chars;
    size_t length;
} VnlS_String;



typedef struct VnlS_StringBuffer {
    char *chars;
    size_t length;
    size_t capacity;
} VnlS_StringBuffer;


typedef struct VnlS_Partition {
    VnlS_String before;
    VnlS_String delim;
    VnlS_String after;
} VnlS_Partition;



typedef enum VnlS_Error {
    VNLS_PARSE_UNDERFLOW,
    VNLS_PARSE_OVERFLOW,
    VNLS_PARSE_NO_NUMBER,
    VNLS_PARSE_NOT_FULL,
} VnlS_Error;



#define VNLS_EMPTY (-1)
#define VNLS_NOTFOUND (-1)



VnlS_String __vnls_do_nothing(VnlS_String);

// Check is `str` is empty.
bool vnls_is_empty(VnlS_String str);

// Check if two strings are equal.
bool vnls_eq(VnlS_String stra, VnlS_String strb);

// Get a substring of `str` at half-open interval `[start, start + length)`.
// First, lshifts to the `start`. Second, rshifts to strip the length down to `length`.
// This ensures that in any case resulting substring is 'part' of the input string.
VnlS_String vnls_substr(VnlS_String str, size_t start, size_t length);

// Left-shift `str` by `min(shift, str.length)` characters.
// Single shift is done as follows:
// ```
//      str.length -= 1;
//      str.chars  += 1;
// ```
// Therefore, it is always safe to work with shifted string as part of the original string.
VnlS_String vnls_lshift(VnlS_String str, size_t shift);

// Right-shift `str` by `min(shift, str.length)` characters.
// Single shift is done as follows:
// ```
//      str.length -= 1;
// ```
// Therefore, it is always safe to work with shifted string as part of the original string.
VnlS_String vnls_rshift(VnlS_String str, size_t shift);

// Check if `str` starts with `prefix`.
bool vnls_startswith(VnlS_String str, VnlS_String prefix);

// Check if `str` ends with `suffix`.
bool vnls_endswith(VnlS_String str, VnlS_String suffix);

// Check if `needle` is present in `haystack`
bool vnls_contains(VnlS_String haystack, VnlS_String needle);

// Count how many times `needle` is present in `haystack`
size_t vnls_count(VnlS_String haystack, VnlS_String needle);

// Find the first occurence of `needle` in the `haystack`.
// Returns index of `needle` or `vnls_NOTFOUND` if not found.
ptrdiff_t vnls_find(VnlS_String haystack, VnlS_String needle);

// Find the last occurence of `needle` in the `haystack`.
// Returns index of `needle` or `vnls_NOTFOUND` if not found.
ptrdiff_t vnls_rfind(VnlS_String haystack, VnlS_String needle);

// Remove leading whitespace characters.
VnlS_String vnls_ltrim(VnlS_String str);

// Remove trailing whitespace characters.
VnlS_String vnls_rtrim(VnlS_String str);

// Remove both leading and trailing whitespace characters.
VnlS_String vnls_trim(VnlS_String str);

// Remove leading characters from specified set.
VnlS_String vnls_ltrim_chars(VnlS_String str, VnlS_String characters);

// Remove trailing characters from specified set.
VnlS_String vnls_rtrim_chars(VnlS_String str, VnlS_String characters);

// Remove both leading and trailing characters from specified set.
VnlS_String vnls_trim_chars(VnlS_String str, VnlS_String characters);

// Remove prefix `prefix` from `str`, if has one.
VnlS_String vnls_remprefix(VnlS_String str, VnlS_String prefix);

// Remove suffix `prefix` from `str`, if has one.
VnlS_String vnls_remsuffix(VnlS_String str, VnlS_String suffix);

// Creates escaped version of `str` in temporary string buffer and returns the resulting string.
// Note that resulting string is not null-terminated.
VnlS_String vnls_escape(VnlS_String str);

// Creates escaped version of `str` in temporary string buffer,
// adds terminating null character and returns the resulting string.
const char *vnls_escapes(VnlS_String str);

// Creates null-terminated version of `str` in temporary string buffer and returns the resulting string.
const char *vnls_to_cstr(VnlS_String str);

// Create `VnlS_String` from null-terminated string.
// Null terminator is not encluded in resulting string.
VnlS_String vnls_from_cstr(const char *);

// Try parse number type from string.
uint8_t     vnls_parse_u8(VnlS_String *, VnlS_Error *);
uint16_t    vnls_parse_u16(VnlS_String *, VnlS_Error *);
uint32_t    vnls_parse_u32(VnlS_String *, VnlS_Error *);
uint64_t    vnls_parse_u64(VnlS_String *, VnlS_Error *);
int8_t      vnls_parse_i8(VnlS_String *, VnlS_Error *);
int16_t     vnls_parse_i16(VnlS_String *, VnlS_Error *);
int32_t     vnls_parse_i32(VnlS_String *, VnlS_Error *);
int64_t     vnls_parse_i64(VnlS_String *, VnlS_Error *);
float       vnls_parse_f32(VnlS_String *, VnlS_Error *);
double      vnls_parse_f64(VnlS_String *, VnlS_Error *);

// Try parse number type, consuming the whole string.
uint8_t     vnls_to_u8(VnlS_String *, VnlS_Error *);
uint16_t    vnls_to_u16(VnlS_String *, VnlS_Error *);
uint32_t    vnls_to_u32(VnlS_String *, VnlS_Error *);
uint64_t    vnls_to_u64(VnlS_String *, VnlS_Error *);
int8_t      vnls_to_i8(VnlS_String *, VnlS_Error *);
int16_t     vnls_to_i16(VnlS_String *, VnlS_Error *);
int32_t     vnls_to_i32(VnlS_String *, VnlS_Error *);
int64_t     vnls_to_i64(VnlS_String *, VnlS_Error *);
float       vnls_to_f32(VnlS_String *, VnlS_Error *);
double      vnls_to_f64(VnlS_String *, VnlS_Error *);

// Chop the first character from the string.
// If string is empty, returns `VNLS_EMPTY`
int vnls_chop(VnlS_String *);

// Splits `str` string into 3 parts:
// - `VnlS_Partition.before`: part before first occurence of `delim`
// - `VnlS_Partition.delim`: `delim` in `str`
// - `VnlS_Partition.after`: part after first occurence of `delim`
// If `delim` is not found, `VnlS_Partition.delim` and `VnlS_Partition.after` will be empty strings,
// with their chars pointing at `str.chars + str.length`
VnlS_Partition vnls_partition(VnlS_String str, VnlS_String delim);

// Splits `str` string into 3 parts:
// - `VnlS_Partition.before`: part before last occurence of `delim`
// - `VnlS_Partition.delim`: `delim` in `str`
// - `VnlS_Partition.after`: part after last occurence of `delim`
// If `delim` is not found, `VnlS_Partition.delim` and `VnlS_Partition.after` will be empty strings,
// with their chars pointing at `str.chars + str.length`
VnlS_Partition vnls_rpartition(VnlS_String str, VnlS_String delim);


// Reset buffer
// Basically sets length to zero.
void vnls_buf_reset(VnlS_StringBuffer *);

// Clear buffer.
// Deallocates memory and sets all fields to zeros.
void vnls_buf_clear(VnlS_StringBuffer *);

// Reserve at least `extra_size` additional elements
void vnls_buf_reserve(VnlS_StringBuffer *, size_t extra_size);

// Reserve exact `size` elements.
// If there are more that `size` elements, does nothing.
void vnls_buf_reserve_exact(VnlS_StringBuffer *, size_t size);

void vnls_buf_push(VnlS_StringBuffer *, char);
void vnls_buf_append(VnlS_StringBuffer *, VnlS_String);
void vnls_buf_appends(VnlS_StringBuffer *, const char *);
void vnls_buf_appendf(VnlS_StringBuffer *, const char *fmt, ...);

// Ensures that contents of the buffer form a null-terminated string
// by adding null in the end, if needed
const char *vnls_buf_cstr(VnlS_StringBuffer *);

// Get string slice of all contents of the buffer
VnlS_String vnls_buf_str(const VnlS_StringBuffer *);

// Read file contents to string buffer
bool vnls_read_file(VnlS_StringBuffer *, VnlS_String filename);




#ifndef VNLS_NO_GENERICS

#define VnlS_String(anystr)                 \
    _Generic((anystr),                      \
        char *: vnls_from_cstr,             \
        VnlS_String: __vnls_do_nothing,     \
        VnlS_StringBuffer *: vnls_buf_str   \
    )(anystr)

#define vnls_is_empty(str)              vnls_is_empty(VnlS_String(str))
#define vnls_eq(stra, strb)             vnls_eq(VnlS_String(stra), VnlS_String(strb))
#define vnls_substr(str, start, length) vnls_substr(VnlS_String(str), start, length)
#define vnls_lshift(str, shift)         vnls_lshift(VnlS_String(str), shift)
#define vnls_rshift(str, shift)         vnls_rshift(VnlS_String(str), shift)
#define vnls_startswith(str, prefix)    vnls_startswith(VnlS_String(str), VnlS_String(prefix))
#define vnls_endswith(str, suffix)      vnls_endswith(VnlS_String(str), VnlS_String(suffix))
#define vnls_find(haystack, needle)     vnls_find(VnlS_String(haystack), VnlS_String(needle))
#define vnls_rfind(heystack, needle)    vnls_rfind(VnlS_String(haystack), VnlS_String(needle))
#define vnls_ltrim(str)                 vnls_ltrim(VnlS_String(str))
#define vnls_rtrim(str)                 vnls_rtrim(VnlS_String(str))
#define vnls_trim(str)                  vnls_trim(VnlS_String(str))
#define vnls_ltrim_chars(str, chars)    vnls_ltrim_chars(VnlS_String(str), chars)
#define vnls_rtrim_chars(str, chars)    vnls_rtrim_chars(VnlS_String(str), chars)
#define vnls_trim_chars(str, chars)     vnls_trim_chars(VnlS_String(str), chars)
#define vnls_remprefix(str, prefix)     vnls_remprefix(VnlS_String(str), VnlS_String(prefix))
#define vnls_remsuffix(str, suffix)     vnls_remsuffix(VnlS_String(str), VnlS_String(suffix))
#define vnls_escape(str)                vnls_escape(VnlS_String(str))
#define vnls_to_u8(str, num)            vnls_to_u8(VnlS_String(str), num)
#define vnls_to_u16(str, num)           vnls_to_u16(VnlS_String(str), num)
#define vnls_to_u32(str, num)           vnls_to_u32(VnlS_String(str), num)
#define vnls_to_u64(str, num)           vnls_to_u64(VnlS_String(str), num)
#define vnls_to_i8(str, num)            vnls_to_i8(VnlS_String(str), num)
#define vnls_to_i16(str, num)           vnls_to_i16(VnlS_String(str), num)
#define vnls_to_i32(str, num)           vnls_to_i32(VnlS_String(str), num)
#define vnls_to_i64(str, num)           vnls_to_i64(VnlS_String(str), num)
#define vnls_to_f32(str, num)           vnls_to_f32(VnlS_String(str), num)
#define vnls_to_f64(str, num)           vnls_to_f64(VnlS_String(str), num)
#define vnls_partition(str, delim)      vnls_partition(VnlS_String(str), VnlS_String(delim))
#define vnls_rpartition(str, delim)     vnls_rpartition(VnlS_String(str), VnlS_String(delim))
#define vnls_contains(haystack, needle) vnls_contains(VnlS_String(haystack), VnlS_String(needle))
#define vnls_count(haystack, needle)    vnls_count(VnlS_String(haystack), VnlS_String(needle))

#define vnls_buf_append(buf, item)                          \
    _Generic((item),                                        \
        char: vnls_buf_push(buf, item),                     \
        default: vnls_buf_append(buf, VnlS_String(item))    \
    )

#define vnls_read_file(buf, filename) vnls_read_file(buf, VnlS_String(filename))


#endif // !VNSL_NO_GENERICS

#endif // VNLSTR_H
