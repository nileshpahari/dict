#include "hashmap.h"
#include <stdlib.h>

#define LOAD_FACTOR 0.75

static unsigned long hash(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
  return hash;
}

static Entry *create_entry(const char *word, const char *meaning) {
  Entry *e = malloc(sizeof(Entry));
  if (!e)
    return NULL;

  e->word = strdup(word);
  e->meaning = strdup(meaning);

  if (!e->word || !e->meaning) {
    free(e->word);
    free(e->meaning);
    free(e);
    return NULL;
  }

  e->next = NULL;
  return e;
}

HashMap *hm_create(size_t capacity) {
  if (!capacity) {
    return NULL;
  }

  HashMap *hm = malloc(sizeof(HashMap));

  if (!hm) {
    return NULL;
  }

  hm->buckets = calloc(capacity, sizeof(Entry *));
  if (!hm->buckets) {
    free(hm);
    return NULL;
  }

  hm->size = 0;
  hm->capacity = capacity;
  return hm;
}

void hm_free(HashMap *hm) {
  if (!hm)
    return;

  for (size_t i = 0; i < hm->capacity; ++i) {
    Entry *curr = hm->buckets[i];
    while (curr) {
      Entry *next = curr->next;
      free(curr->word);
      free(curr->meaning);
      free(curr);
      curr = next;
    }
  }

  free(hm->buckets);
  free(hm);
}

static bool hm_resize(HashMap *hm) {
  size_t new_cap = hm->capacity * 2;
  Entry **new_buckets = calloc(new_cap, sizeof(Entry *));
  if (!new_buckets) {
    return false;
  }

  for (size_t i = 0; i < hm->capacity; i++) {
    Entry *curr = hm->buckets[i];
    while (curr) {
      Entry *next = curr->next;
      unsigned long idx = hash(curr->word) % new_cap;

      curr->next = new_buckets[idx];
      new_buckets[idx] = curr;

      curr = next;
    }
  }

  free(hm->buckets);
  hm->buckets = new_buckets;
  hm->capacity = new_cap;
  return true;
}

bool hm_add(const char *word, const char *meaning, HashMap *hm) {
  if (!hm || !word || !meaning)
    return false;

  if ((double)hm->size / hm->capacity > LOAD_FACTOR) {
    if (!hm_resize(hm))
      return false;
  }

  unsigned long idx = hash(word) % hm->capacity;
  Entry *curr = hm->buckets[idx];

  while (curr) {
    if (strcmp(curr->word, word) == 0) {
      char *new_meaning = strdup(meaning);
      if (!new_meaning)
        return false;
      free(curr->meaning);
      curr->meaning = new_meaning;
      return true;
    }
    curr = curr->next;
  }

  Entry *new_entry = create_entry(word, meaning);
  if (!new_entry)
    return false;

  new_entry->next = hm->buckets[idx];
  hm->buckets[idx] = new_entry;
  hm->size++;

  return true;
}

bool hm_delete(const char *word, HashMap *hm) {
  if (!hm || !word) {
    return false;
  }

  unsigned long idx = hash(word) % hm->capacity;
  Entry *curr = hm->buckets[idx];
  Entry *prev = NULL;

  while (curr) {

    if (strcmp(curr->word, word) == 0) {
      if (prev) {
        prev->next = curr->next;
      } else {
        hm->buckets[idx] = curr->next;
      }

      free(curr->word);
      free(curr->meaning);
      free(curr);

      hm->size--;
      return true;
    }

    prev = curr;
    curr = curr->next;
  }

  return false;
}

const char *hm_get(const char *word, const HashMap *hm) {
  if (!hm || !word)
    return NULL;

  unsigned long idx = hash(word) % hm->capacity;
  Entry *curr = hm->buckets[idx];

  while (curr) {
    if (strcmp(curr->word, word) == 0) {
      return curr->meaning;
    }
    curr = curr->next;
  }

  return NULL;
}
