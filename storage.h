#ifndef STORAGE_H
#define STORAGE_H

#include "hashmap.h"
#include <stdbool.h> 

bool load_from_file(HashMap* dict, const char *file);
bool save_to_file(HashMap* dict, const char* file);

#endif


