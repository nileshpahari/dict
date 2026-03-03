#ifndef CLI_H
#define CLI_H

#include "hashmap.h"

int cmd_mode(int argc, char** argv, HashMap* dict);

int interactive_mode(HashMap* dict);

#endif
