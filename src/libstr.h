/*
 * Copyright (c) 2013 Konsta Kokkinen <kray@tsundere.fi>
 *
 * libstr is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */
#ifndef LIBSTR_H
#define LIBSTR_H

#include <string.h>


/** Called when memory allocation fails. Default is libstr_panic_func */
extern void (*libstr_panic)();

/** Outputs "libstr: out of memory" and calls abort */
void libstr_panic_func();



/**
 * General container for any kind of buffer data, such as strings. The
 * implementation doesn't care about what the data stored actually is, but it 
 * ensures the data is always null terminated
 */
typedef struct STR *STR;



/** Returns new allocated STR */
STR STR_new();

/** Returns new STR using @p string, which is invalidated, as its buffer */
STR STR_of(char *string);

/** Returns new allocated STR with @p string content copied to it */
STR STR_from(const char *string);

/** Returns new allocated STR with variable argument formatted @p format copied to it */
STR STR_fromf(const char *format, ...);

/** Frees @p STR and returns its content string, which must be freed by user */
char *STR_release(STR str);

/** Frees a STR */
void STR_free(STR str);



/** Returns the internal null terminated string from @p str */
char *STR_string(STR str);

/** Returns how many bytes @p str is in length */
size_t STR_size(STR str);



/** Replaces @p str content with null terminated @p string */
void STR_set(STR str, const char *string);

/** Replaces @p str content with @p size bytes of @p string */
void STR_nset(STR str, const char *string, size_t size);


/** Inserts null terminated @p string to @p str at @p pos bytes */
void STR_insert(STR str, size_t pos, const char *string);

/** Inserts @p size bytes of @p string to  @p str at @p pos bytes */
void STR_ninsert(STR str, size_t pos, const char *string, size_t size);

/** Inserts variable argument formatted @p format to @p str at @p pos bytes */
void STR_insertf(STR str, size_t pos, const char *format, ...);


/** Appends null terminated @p string to @p str */
void STR_append(STR str, const char *string);

/** Appends @p size bytes from @p string to @p str */
void STR_nappend(STR str, const char *string, size_t size);

/** Appends variable argument formatted @p format to @p str */
void STR_appendf(STR str, const char *format, ...);


/** Prepends null terminated @p string to @p str */
void STR_prepend(STR str, const char *string);

/** Prepends @p size bytes from @p string to @p str */
void STR_nprepend(STR str, const char *string, size_t size);

/** Prepends variable argument formatted @p format to @p str */
void STR_prependf(STR str, const char *format, ...);



/** Empties @p str contents */
void STR_clear(STR str);

/** Erases @p size bytes starting from @p pos bytes from @p str */
void STR_erase(STR str, size_t pos, size_t size);

#endif
