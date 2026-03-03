#include "cli.h"
#include "hashmap.h"
#include "storage.h"

int main(int argc, char **argv) {

  HashMap *dict = hm_create(100);
  if (!dict)
    return 1;

  if(!load_from_file(dict, "dict.txt")) return 1;

  int status = 0;

  if (argc == 1) {
	interactive_mode(dict);
  } else {
	int success = cmd_mode(argc, argv, dict);
  }

  if(!save_to_file(dict, "dict.txt")) return 1;
  hm_free(dict);

  return status;
}
