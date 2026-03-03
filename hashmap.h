#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

bool hm_add(const char *word, const char *meaning, HashMap *hm);

bool hm_delete(const char *word, HashMap *hm);

const char *hm_get(const char *word, const HashMap *hm);

#endif
