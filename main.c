#include "cli.h"
#include "hashmap.h"
#include "storage.h"

#define FILENAME "dict.txt"

int main(int argc, char **argv) {

  HashMap *dict = hm_create(100);
  if (!dict)
    return 1;

  if (!load_from_file(dict, FILENAME)) {
    hm_free(dict);
    return 1;
  }

  int status = 0;

  if (argc == 1) {
    status = interactive_mode(dict);
  } else {
    status = cmd_mode(argc, argv, dict);
  }

  if (!save_to_file(dict, FILENAME)) {
    hm_free(dict);
	return 1;
  }

  hm_free(dict);
  return status;
}
