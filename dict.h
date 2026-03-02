#ifndef DICT_H
#define DICT_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Entry {
  char *word;
  char *meaning;
  struct Entry *next;
} Entry;

typedef struct {
  Entry **buckets;
  size_t capacity;
  size_t size;
} HashMap;

HashMap *hm_create(size_t capacity);

void hm_free(HashMap *hm);

Entry* hm_add(HashMap *hm, const char *word, const char *meaning);

void show_help();

bool add_entry(const char *word, const char *meaning);

bool delete_entry(const char *word);

Entry *search_entry(const char *word);

void show_entry(Entry *e);

#endif
