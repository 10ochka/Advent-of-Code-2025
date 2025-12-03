
#define VNLS_NO_GENERICS
#include "vnls.h"

#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

#define min(a, b) ((a) < (b)) ? (a) : (b)
#define max(a, b) ((a) > (b)) ? (a) : (b)




// 'Safer' version of standard `malloc`. Panics if `malloc` returned `NULL`.
// Therefore the result of this function is a valid pointer.
void *vnls_malloc(size_t size);

// 'Safer' version of standard `realloc`. Panics if `realloc` returned `NULL`.
// Therefore the result of this function is a valid pointer.
// On failure, frees `ptr`, and only then panics.
void *vnls_realloc(void *ptr, size_t new_size);

// 'Safer' version of standard `free`.
// If `NULL` is passed, does nothing.
void vnls_free(void *ptr);


void vnls_panicf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\033[31m");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\033[0m");
    va_end(args);
    abort();
}



size_t vnls_align(size_t x) {
    const size_t WORD_SIZE = sizeof(size_t);
    return (x + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1);
}


void *vnls_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        vnls_panicf("Cannot vnls_malloc!\n");
    }
    memset(ptr, 0, size);
    return ptr;
}

void *vnls_realloc(void *ptr, size_t new_size) {
    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL) {
        vnls_free(ptr);
        vnls_panicf("Cannot vnls_realloc!\n");
    }
    return new_ptr;
}

void vnls_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}









VnlS_String __vnls_do_nothing(VnlS_String str) {
    return str;
}


bool vnls_is_empty(VnlS_String str) {
    return str.length == 0;
}


bool vnls_eq(VnlS_String stra, VnlS_String strb) {
    if (stra.length != strb.length) {
        return false;
    }

    return memcmp(
        stra.chars,
        strb.chars,
        stra.length
    ) == 0;
}


int vnls_at(VnlS_String str, ptrdiff_t pos) {
    if (str.length <= pos || pos < -str.length) {
        return VNLS_OUTOFBOUNDS;
    }

    if (pos >= 0) {
        return str.chars[pos];
    } else {
        return str.chars[str.length-pos];
    }
}


VnlS_String vnls_substr(VnlS_String str, size_t start, size_t length) {
    str = vnls_lshift(str, start);
    if (str.length > length) {
        str = vnls_rshift(str, str.length - length);
    }
    return str;
}


VnlS_String vnls_lshift(VnlS_String str, size_t shift) {
    shift = min(str.length, shift);
    str.length -= shift;
    str.chars  += shift;
    return str;
}


VnlS_String vnls_rshift(VnlS_String str, size_t shift) {
    shift = min(str.length, shift);
    str.length -= shift;
    return str;
}


bool vnls_startswith(VnlS_String str, VnlS_String prefix) {
    if (str.length < prefix.length) {
        return false;
    }
    VnlS_String true_prefix = vnls_substr(str, 0, prefix.length);
    return vnls_eq(true_prefix, prefix);
}


bool vnls_endswith(VnlS_String str, VnlS_String suffix) {
    if (str.length < suffix.length) {
        return false;
    }
    VnlS_String true_suffix = vnls_substr(str, str.length - suffix.length, suffix.length);
    return vnls_eq(true_suffix, suffix);
}


bool vnls_contains(VnlS_String haystack, VnlS_String needle) {
    return vnls_count(haystack, needle) > 0;
}


size_t vnls_count(VnlS_String haystack, VnlS_String needle) {
    size_t count = 0;
    while (haystack.length) {
        ptrdiff_t idx = vnls_find(haystack, needle);
        if (idx != VNLS_NOTFOUND) {
            count += 1;
            haystack = vnls_lshift(haystack, idx + needle.length);
        } else {
            break;
        }
    }
    return count;
}


ptrdiff_t vnls_find(VnlS_String haystack, VnlS_String needle) {
    if (vnls_is_empty(needle)) {
        return 0;
    }

    if (haystack.length < needle.length) {
        return VNLS_NOTFOUND;
    }

    for (size_t i = 0; 0 <= haystack.length - needle.length; ++i) {
        if (vnls_startswith(haystack, needle)) {
            return i;
        }
        haystack = vnls_lshift(haystack, 1);
    }

    return VNLS_NOTFOUND;
}


ptrdiff_t vnls_rfind(VnlS_String haystack, VnlS_String needle) {
    if (vnls_is_empty(needle)) {
        return haystack.length;
    }

    if (haystack.length < needle.length) {
        return VNLS_NOTFOUND;
    }

    for (size_t i = 0; i < haystack.length - needle.length; ++i) {
        if (vnls_endswith(haystack, needle)) {
            return i;
        }
        haystack = vnls_rshift(haystack, 1);
    }

    return VNLS_NOTFOUND;
}


VnlS_String vnls_ltrim(VnlS_String str) {
    for (size_t i = 0; i < str.length; ++i) {
        char fist_char = str.chars[0];
        if (isspace(fist_char)) {
            str = vnls_lshift(str, 1);
        } else {
            break;
        }
    }

    return str;
}


VnlS_String vnls_rtrim(VnlS_String str) {
    for (size_t i = 0; i < str.length; ++i) {
        char last_char = str.chars[str.length - 1];
        if (isspace(last_char)) {
            str = vnls_rshift(str, 1);
        } else {
            break;
        }
    }

    return str;
}


VnlS_String vnls_trim(VnlS_String str) {
    return vnls_ltrim(
        vnls_rtrim(str)
    );
}


VnlS_String vnls_ltrim_chars(VnlS_String str, VnlS_String characters) {
    char characters_cstr[characters.length + 1];
    strncpy(characters_cstr, characters.chars, characters.length);
    characters_cstr[characters.length] = '\0';

    for (size_t i = 0; i < str.length; ++i) {
        char fist_char = str.chars[0];
        if (strchr(characters_cstr, fist_char) != NULL) {
            str = vnls_lshift(str, 1);
        }
    }

    return str;
}


VnlS_String vnls_rtrim_chars(VnlS_String str, VnlS_String characters) {
    char characters_cstr[characters.length + 1];
    strncpy(characters_cstr, characters.chars, characters.length);
    characters_cstr[characters.length] = '\0';

    for (size_t i = 0; i < str.length; ++i) {
        char last_char = str.chars[str.length - 1];
        if (strchr(characters_cstr, last_char) != NULL) {
            str = vnls_rshift(str, 1);
        }
    }

    return str;
}


VnlS_String vnls_trim_chars(VnlS_String str, VnlS_String characters) {
    return vnls_ltrim_chars(
        vnls_rtrim_chars(str, characters),
        characters
    );
}


VnlS_String vnls_remprefix(VnlS_String str, VnlS_String prefix) {
    if (vnls_startswith(str, prefix)) {
        str = vnls_lshift(str, prefix.length);
    }

    return str;
}


VnlS_String vnls_remsuffix(VnlS_String str, VnlS_String suffix) {
    if (vnls_endswith(str, suffix)) {
        str = vnls_rshift(str, suffix.length);
    }

    return str;
}




VnlS_StringBuffer VNLS_TEMP_BUF = {0};


VnlS_String vnls_escape(VnlS_String str) {
    vnls_buf_reset(&VNLS_TEMP_BUF);
    VnlS_StringBuffer *buffer = &VNLS_TEMP_BUF;

    vnls_buf_push(buffer, '"');
    for (size_t i = 0; i < str.length; ++i) {
        char c = str.chars[i];
        switch (c) {
        case '\'': vnls_buf_appends(buffer, "\\'"); break;
        case '\"': vnls_buf_appends(buffer, "\\\""); break;
        // case '\?': vnls_buf_append(buffer, "\\?"); break;
        case '\\': vnls_buf_appends(buffer, "\\\\"); break;
        case '\a': vnls_buf_appends(buffer, "\\a"); break;
        case '\b': vnls_buf_appends(buffer, "\\b"); break;
        case '\f': vnls_buf_appends(buffer, "\\f"); break;
        case '\n': vnls_buf_appends(buffer, "\\n"); break;
        case '\r': vnls_buf_appends(buffer, "\\r"); break;
        case '\t': vnls_buf_appends(buffer, "\\t"); break;
        case '\v': vnls_buf_appends(buffer, "\\v"); break;
        default:   vnls_buf_push(buffer, c); break;
        }
    }

    vnls_buf_push(buffer, '"');
    vnls_buf_push(buffer, '\0');

    return (VnlS_String){
        VNLS_TEMP_BUF.chars,
        VNLS_TEMP_BUF.length,
    };
}



const char *vnls_escapes(VnlS_String str) {
    return vnls_escape(str).chars;
}


const char *vnls_to_cstr(VnlS_String str) {
    vnls_buf_reset(&VNLS_TEMP_BUF);
    vnls_buf_append(&VNLS_TEMP_BUF, str);
    vnls_buf_push(&VNLS_TEMP_BUF, '\0');
    return VNLS_TEMP_BUF.chars;
}



VnlS_String vnls_from_cstr(const char *cstr) {
    return (VnlS_String){ cstr, strlen(cstr) };
}



int getbase(VnlS_String str) {
    str = vnls_ltrim(str);
    if (vnls_startswith(str, vnls_from_cstr("+"))
     || vnls_startswith(str, vnls_from_cstr("-"))) {
         str = vnls_lshift(str, 1);
    }

    int base;
    if (vnls_startswith(str, vnls_from_cstr("0x"))
     || vnls_startswith(str, vnls_from_cstr("0X"))) {
        base = 16;
    } else if (vnls_startswith(str, vnls_from_cstr("0b"))
            || vnls_startswith(str, vnls_from_cstr("0B"))) {
        base = 2;
    } else if (vnls_startswith(str, vnls_from_cstr("0o"))
            || vnls_startswith(str, vnls_from_cstr("0O"))) {
        base = 8;
    } else {
        base = 10;
    }

    return base;
}

uintmax_t parse_uint(VnlS_String *str, uintmax_t maxval, VnlS_Error *err, bool full) {
    int base = getbase(*str);
    VnlS_Error _dummy_error;
    if (err == NULL) {
        err = &_dummy_error;
    }

    char *cstr = vnls_malloc(str->length + 1);
    memcpy(cstr, str->chars, str->length);
    cstr[str->length] = '\0';
    char *end;
    uintmax_t number = strtoumax(cstr, &end, base);

    int _errno = errno;
    if (_errno == EINVAL) {
        *err = VNLS_PARSE_NO_NUMBER;
    }
    if (_errno == ERANGE || number > maxval) {
        *err = VNLS_PARSE_OVERFLOW;
    }

    if (end == cstr) {
        *err = VNLS_PARSE_NO_NUMBER;
    }

    *str = vnls_lshift(*str, end - cstr);

    if (full && !vnls_is_empty(*str)) {
        *err = VNLS_PARSE_NOT_FULL;
    }

    vnls_free(cstr);
    return number;
}

intmax_t parse_int(VnlS_String *str, intmax_t minval, intmax_t maxval, VnlS_Error *err, bool full) {
    int base = getbase(*str);
    VnlS_Error _dummy_error;
    if (err == NULL) {
        err = &_dummy_error;
    }

    char *cstr = vnls_malloc(str->length + 1);
    memcpy(cstr, str->chars, str->length);
    cstr[str->length] = '\0';
    char *end;
    intmax_t number = strtoimax(cstr, &end, base);

    int _errno = errno;
    if (_errno == EINVAL) {
        *err = VNLS_PARSE_NO_NUMBER;
    }

    if (_errno == ERANGE || number > maxval || number < minval) {
        if (number > maxval) *err = VNLS_PARSE_OVERFLOW;
        if (number < minval) *err = VNLS_PARSE_UNDERFLOW;
        *err = VNLS_PARSE_OVERFLOW;
    }

    *str = vnls_lshift(*str, end - cstr);

    if (full && !vnls_is_empty(*str)) {
        *err = VNLS_PARSE_NOT_FULL;
    }

    vnls_free(cstr);
    return number;
}

intmax_t parse_float(VnlS_String *str, double minval, double maxval, VnlS_Error *err, bool full) {
    VnlS_Error _dummy_error;
    if (err == NULL) {
        err = &_dummy_error;
    }

    char *cstr = vnls_malloc(str->length + 1);
    memcpy(cstr, str->chars, str->length);
    cstr[str->length] = '\0';
    char *end;
    double number = strtod(cstr, &end);

    int _errno = errno;
    if (_errno == EINVAL) {
        *err = VNLS_PARSE_NO_NUMBER;
    }

    if (_errno == ERANGE || number > maxval || number < minval) {
        if (number > maxval) *err = VNLS_PARSE_OVERFLOW;
        if (number < minval) *err = VNLS_PARSE_UNDERFLOW;
        *err = VNLS_PARSE_OVERFLOW;
    }

    *str = vnls_lshift(*str, end - cstr);

    if (full && !vnls_is_empty(*str)) {
        *err = VNLS_PARSE_NOT_FULL;
    }

    vnls_free(cstr);
    return number;
}


uint8_t vnls_parse_u8(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT8_MAX, err, false); }
uint16_t    vnls_parse_u16(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT16_MAX, err, false); }
uint32_t    vnls_parse_u32(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT32_MAX, err, false); }
uint64_t    vnls_parse_u64(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT64_MAX, err, false); }
int8_t      vnls_parse_i8(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT8_MIN, INT8_MAX, err, false); }
int16_t     vnls_parse_i16(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT16_MIN, INT16_MAX, err, false); }
int32_t     vnls_parse_i32(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT32_MIN, INT32_MAX, err, false); }
int64_t     vnls_parse_i64(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT64_MIN, INT32_MAX, err, false); }
float       vnls_parse_f32(VnlS_String *str, VnlS_Error *err) { return parse_float(str, FLT_MIN, FLT_MAX, err, false); }
double      vnls_parse_f64(VnlS_String *str, VnlS_Error *err) { return parse_float(str, DBL_MIN, DBL_MAX, err, false); }


uint8_t     vnls_to_u8(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT8_MAX, err, true); }
uint16_t    vnls_to_u16(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT16_MAX, err, true); }
uint32_t    vnls_to_u32(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT32_MAX, err, true); }
uint64_t    vnls_to_u64(VnlS_String *str, VnlS_Error *err) { return parse_uint(str, UINT64_MAX, err, true); }
int8_t      vnls_to_i8(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT8_MIN, INT8_MAX, err, true); }
int16_t     vnls_to_i16(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT16_MIN, INT16_MAX, err, true); }
int32_t     vnls_to_i32(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT32_MIN, INT32_MAX, err, true); }
int64_t     vnls_to_i64(VnlS_String *str, VnlS_Error *err) { return parse_int(str, INT64_MIN, INT32_MAX, err, true); }
float       vnls_to_f32(VnlS_String *str, VnlS_Error *err) { return parse_float(str, FLT_MIN, FLT_MAX, err, true); }
double      vnls_to_f64(VnlS_String *str, VnlS_Error *err) { return parse_float(str, DBL_MIN, DBL_MAX, err, true); }


int vnls_chop(VnlS_String *str) {
    if (vnls_is_empty(*str)) {
        return VNLS_EMPTY;
    }
    char c = str->chars[0];
    *str = vnls_lshift(*str, 1);
    return c;
}



VnlS_Partition vnls_partition(VnlS_String str, VnlS_String delim) {
    ptrdiff_t delim_start = vnls_find(str, delim);

    if (delim_start == VNLS_NOTFOUND) {
        return (VnlS_Partition) {
            .before = str,
            .delim = vnls_substr(str, str.length, 0),
            .after = vnls_substr(str, str.length, 0),
        };
    } else {
        return (VnlS_Partition) {
            .before = vnls_substr(str, 0, delim_start),
            .delim = vnls_substr(str, delim_start, delim.length),
            .after = vnls_substr(str, delim_start + delim.length, SIZE_T_MAX),
        };
    }
}



VnlS_Partition vnls_rpartition(VnlS_String str, VnlS_String delim) {
    ptrdiff_t delim_start = vnls_rfind(str, delim);

    if (delim_start == VNLS_NOTFOUND) {
        return (VnlS_Partition) {
            .before = str,
            .delim = vnls_substr(str, str.length, 0),
            .after = vnls_substr(str, str.length, 0),
        };
    } else {
        return (VnlS_Partition) {
            .before = vnls_substr(str, 0, delim_start),
            .delim = vnls_substr(str, delim_start, delim.length),
            .after = vnls_substr(str, delim_start + delim.length, SIZE_T_MAX),
        };
    }
}



void vnls_buf_reset(VnlS_StringBuffer *buf) {
    buf->length = 0;
}



void vnls_buf_clear(VnlS_StringBuffer *buf) {
    vnls_free(buf->chars);
    *buf = (VnlS_StringBuffer){0};
}


void vnls_buf_reserve(VnlS_StringBuffer *buf, size_t extra_size) {
    size_t min_capacity = buf->length + extra_size;
    if (buf->capacity < min_capacity) {
        size_t new_capacity = buf->capacity
                            ? vnls_align(max(buf->capacity * 2, min_capacity))
                            : vnls_align(min_capacity);
        buf->chars = vnls_realloc(buf->chars, new_capacity);
        buf->capacity = new_capacity;
    }
}



void vnls_buf_reserve_exact(VnlS_StringBuffer *buf, size_t size) {
    if (buf->capacity < size) {
        buf->chars = vnls_realloc(buf->chars, size);
        buf->capacity = size;
    }
}

void vnls_buf_push(VnlS_StringBuffer *buf, char c) {
    vnls_buf_reserve(buf, 1);
    buf->chars[buf->length++] = c;
}

void vnls_buf_append(VnlS_StringBuffer *buf, VnlS_String str) {
    vnls_buf_reserve(buf, str.length);
    memcpy(buf->chars + buf->length, str.chars, str.length);
    buf->length += str.length;
}

void vnls_buf_appends(VnlS_StringBuffer *buf, const char *cstr) {
    size_t len = strlen(cstr);
    vnls_buf_reserve(buf, len);
    memcpy(buf->chars + buf->length, cstr, len);
    buf->length += len;
}
void vnls_buf_appendf(VnlS_StringBuffer *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    size_t extra_size = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    // +1 for terminating null
    vnls_buf_reserve(buf, extra_size + 1);
    vsnprintf(buf->chars + buf->length, extra_size, fmt, args);
    buf->length += extra_size;
    va_end(args);
}



const char *vnls_buf_cstr(VnlS_StringBuffer *buf) {
    // empty or last char is not null
    if (!buf->length || !buf->chars[buf->length - 1]) {
        vnls_buf_push(buf, '\0');
    }
    return buf->chars;
}


VnlS_String vnls_buf_str(const VnlS_StringBuffer *buf) {
    return (VnlS_String){ buf->chars, buf->length };
}


bool vnls_read_file(VnlS_StringBuffer *buf, VnlS_String filename) {

    FILE *fp = fopen(vnls_to_cstr(filename), "r");
    if (!fp) return false;

    const size_t CHUNKSIZE = 1024;
    rewind(fp);
    while (true) {
        vnls_buf_reserve(buf, CHUNKSIZE);
        char f[1024] = {0};
        size_t chars_read = fread(
            f,
            sizeof(char),
            CHUNKSIZE,
            fp);
        vnls_buf_append(buf, (VnlS_String){f, chars_read});

        if (!chars_read) break;
    }

    return true;
}
