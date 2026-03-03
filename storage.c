#include "storage.h"
#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_FILE_LOAD "Could not read the dictionary file.\n"
#define ERR_FILE_SAVE "Could not write to the dictionary file.\n"

bool load_from_file(HashMap *dict, const char *file) {
  if (!dict || !file) {
    fprintf(stderr, ERR_FILE_LOAD);
    return false;
  }

  FILE *fp = fopen(file, "r");
  if (!fp) {
    return true;
  }

  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, fp) != -1) {
    line[strcspn(line, "\n")] = 0;

    char *tab = strchr(line, '\t');
    if (!tab)
      continue;

    *tab = '\0';
    char *word = line;
    char *meaning = tab + 1;

    if (*word && *meaning)
      hm_add(word, meaning, dict);
  }

  free(line);
  fclose(fp);
  return true;
}

bool save_to_file(HashMap *dict, const char *file) {
  if (!dict || !file) {
    fprintf(stderr, ERR_FILE_LOAD);
    return false;
  }

  const char *tmpname = "dict.tmp";

  FILE *fp = fopen(tmpname, "w");
  if (!fp) {
    fprintf(stderr, ERR_FILE_LOAD);
    return false;
  }

  for (size_t i = 0; i < dict->capacity; i++) {
    Entry *curr = dict->buckets[i];
    while (curr) {
      fprintf(fp, "%s\t%s\n", curr->word, curr->meaning);
      curr = curr->next;
    }
  }

  fclose(fp);

  if (rename(tmpname, file) != 0) {
    fprintf(stderr, ERR_FILE_LOAD);
    return false;
  }

  return true;
}
