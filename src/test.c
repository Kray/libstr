/*
 * Copyright (c) 2013 Konsta Kokkinen <kray@tsundere.fi>
 *
 * libstr is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */
#include "libstr.h"

#include <assert.h>
#include <stdlib.h>

/* Mostly comprehensive "unit tests" for libstr */
int main()
{
  STR str;
  char *string;
  
  str = STR_new();
  assert(STR_size(str) == 0);
  STR_free(str);
  
  str = STR_of(strdup("test"));
  assert(STR_size(str) == 4);
  assert(strcmp(STR_string(str), "test") == 0);
  STR_clear(str);
  assert(STR_size(str) == 0);
  STR_free(str);
  
  string = strdup("abcdefghijklmnopqrstuvwxyz");
  str = STR_from(string);
  assert(strcmp(STR_string(str), string) == 0);
  STR_free(str);
  
  str = STR_fromf("%d %s test", 12, string);
  assert(STR_size(str) == 34);
  assert(strcmp(STR_string(str), "12 abcdefghijklmnopqrstuvwxyz test") == 0);
  
  free(string);
  string = STR_release(str);
  assert(strcmp(string, "12 abcdefghijklmnopqrstuvwxyz test") == 0);
  free(string);
  
  str = STR_new();
  
  STR_set(str, "test");
  assert(strcmp(STR_string(str), "test") == 0);
  
  STR_nset(str, "test", 2);
  assert(strcmp(STR_string(str), "te") == 0);
  
  STR_clear(str);
  
  STR_append(str, "aa");
  assert(strcmp(STR_string(str), "aa") == 0);
  STR_nappend(str, "bbbb", 3);
  assert(strcmp(STR_string(str), "aabbb") == 0);
  STR_appendf(str, " %d ", 999);
  assert(strcmp(STR_string(str), "aabbb 999 ") == 0);
  
  STR_prepend(str, "cc");
  assert(strcmp(STR_string(str), "ccaabbb 999 ") == 0);
  STR_nprepend(str, "ddd", 1);
  assert(strcmp(STR_string(str), "dccaabbb 999 ") == 0);
  STR_prependf(str, "e%de", 8);
  assert(strcmp(STR_string(str), "e8edccaabbb 999 ") == 0);
  
  STR_clear(str);
  
  STR_insert(str, 0, "test");
  assert(strcmp(STR_string(str), "test") == 0);
  STR_insert(str, 0, "aa");
  assert(strcmp(STR_string(str), "aatest") == 0);
  STR_insert(str, 6, "bb");
  assert(strcmp(STR_string(str), "aatestbb") == 0);
  STR_insert(str, 2, "c");
  assert(strcmp(STR_string(str), "aactestbb") == 0);
  STR_ninsert(str, 3, "test", 2);
  assert(strcmp(STR_string(str), "aactetestbb") == 0);
  STR_insertf(str, 3, "%d", 10);
  assert(strcmp(STR_string(str), "aac10tetestbb") == 0);
  
  STR_erase(str, 2, 9);
  assert(strcmp(STR_string(str), "aabb") == 0);
  
  STR_free(str);
  
  return 0;
}
