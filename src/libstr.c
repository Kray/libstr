/*
 * Copyright (c) 2013 Konsta Kokkinen <kray@tsundere.fi>
 *
 * libstr is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */
#include "libstr.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void (*libstr_panic)() = libstr_panic_func;

void libstr_panic_func()
{
  fprintf(stderr, "libstr: out of memory\n");
  abort();
}



static void *alloc_(size_t size)
{
  void *result = malloc(size);
  if (!result) {
    libstr_panic();
  }
  return result;
}

static void *realloc_(void *ptr, size_t size)
{
  void *result = realloc(ptr, size);
  if (!result) {
    libstr_panic();
  }
  return result;
}

static void free_(void *ptr)
{
  free(ptr);
}



#define BUILD_VARGS_STRING \
  va_list va_args; \
  size_t va_size; \
  char *va_buf; \
  \
  va_start(va_args, format); \
  va_size = vsnprintf(NULL, 0, format, va_args); \
  va_end(va_args); \
  \
  va_buf = alloc_(va_size + 1); \
  \
  va_start(va_args, format); \
  vsnprintf(va_buf, va_size + 1, format, va_args); \
  va_end(va_args);



struct STR {
  char *data;
  size_t size;
  size_t max_size;
};



static STR new_STR()
{
  return alloc_(sizeof(struct STR));
}

STR STR_new()
{
  STR str = new_STR();
  str->data = NULL;
  str->size = 0;
  str->max_size = 0;
  return str;
}

STR STR_of(char *string)
{
  STR str = new_STR();
  str->data = string;
  str->size = strlen(string);
  str->max_size = str->size;
  return str;
}

STR STR_from(const char *string)
{
  STR str = new_STR();
  str->size = strlen(string);
  str->max_size = str->size;
  str->data = alloc_(str->size + 1);
  
  memcpy(str->data, string, str->size);
  str->data[str->size] = '\0';
  return str;
}

STR STR_fromf(const char *format, ...)
{
  BUILD_VARGS_STRING
  
  STR str = new_STR();
  str->data = va_buf;
  str->size = va_size;
  str->max_size = str->size;
  return str;
}

char *STR_release(STR str)
{
  char *result = str->data;
  free_(str);
  return result;
}

void STR_free(STR str)
{
  free_(str->data);
  free_(str);
}



char *STR_string(STR str)
{
  return str->data;
}

size_t STR_size(STR str)
{
  return str->size;
}



static void ensure_space(STR str, size_t size)
{
  if (str->max_size >= size) {
    return;
  }
  
  if (str->max_size < 8) {
    str->max_size = 8;
  }
  str->max_size = (str->max_size * 2) > size ? str->max_size * 2 : size;
  str->data = realloc_(str->data, str->max_size + 1);
}

void STR_set(STR str, const char *string)
{
  STR_nset(str, string, strlen(string));
}

void STR_nset(STR str, const char *string, size_t size)
{
  ensure_space(str, size);
  
  memcpy(str->data, string, size);
  
  str->size = size;
  
  str->data[str->size] = '\0';
}


void STR_insert(STR str, size_t pos, const char* string)
{
  STR_ninsert(str, pos, string, strlen(string));
}

void STR_ninsert(STR str, size_t pos, const char* string, size_t size)
{
  ensure_space(str, str->size + size);
  
  if (pos == 0) {
    STR_nprepend(str, string, size);
    return;
  }
  
  if (pos == str->size) {
    STR_nappend(str, string, size);
    return;
  }
  
  memmove(str->data + pos + size, str->data + pos, str->size - pos);
  memcpy(str->data + pos, string, size);
  
  str->size += size;
  
  str->data[str->size] = '\0';
}

void STR_insertf(STR str, size_t pos, const char *format, ...)
{
  BUILD_VARGS_STRING
  
  STR_ninsert(str, pos, va_buf, va_size);
  free_(va_buf);
}


void STR_append(STR str, const char *string)
{
  STR_nappend(str, string, strlen(string));
}

void STR_nappend(STR str, const char* string, size_t size)
{
  ensure_space(str, str->size + size);
  
  memcpy(str->data + str->size, string, size);
  
  str->size += size;
  
  str->data[str->size] = '\0';
}

void STR_appendf(STR str, const char *format, ...)
{
  BUILD_VARGS_STRING
  
  STR_nappend(str, va_buf, va_size);
  free_(va_buf);
}


void STR_prepend(STR str, const char *string)
{
  STR_nprepend(str, string, strlen(string));
}

void STR_nprepend(STR str, const char* string, size_t size)
{
  ensure_space(str, str->size + size);
  
  memmove(str->data + size, str->data, str->size);
  memcpy(str->data, string, size);
  
  str->size += size;
  
  str->data[str->size] = '\0';
}

void STR_prependf(STR str, const char *format, ...)
{
  BUILD_VARGS_STRING
  
  STR_nprepend(str, va_buf, va_size);
  free_(va_buf);
}



void STR_clear(STR str)
{
  str->size = 0;
  str->data[0] = '\0';
}

void STR_erase(STR str, size_t pos, size_t size)
{
  memmove(str->data + pos, str->data + pos + size, str->size - pos - size);
  
  str->size -= size;
  
  str->data[str->size] = '\0';
}
