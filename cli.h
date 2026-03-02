#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

void show_help();

bool add_entry(const char *word, const char *meaning);

bool delete_entry(const char *word);

const char* search_entry(const char *word);

void show_entry(const char* word, const char* meaning);

#endif
