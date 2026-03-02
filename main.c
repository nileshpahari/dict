#include "dict.h"
#include <stdio.h>
#include <string.h>

#define ERR_TOO_MANY "Too many arguments.\n"
#define ERR_TOO_FEW "Too few arguments.\n"
#define ERR_ADD_FAILURE "Failed to add the word.\n"
#define ERR_DELETE_FAILURE "Failed to delete the word.\n"

#define MSG_ADD_SUCCESS "Word added successfully.\n"
#define MSG_DELETE_SUCCESS "Word deleted successfully.\n"

int main(int argc, char **argv) {

  if (argc < 2) {
    show_help();
    return 0;
  }

  const char *cmd = argv[1];

  if (strcmp(cmd, "help") == 0) {
    if (argc > 2) {
      fprintf(stderr, ERR_TOO_MANY);
      return 1;
    }
    show_help();
  }

  else if (strcmp(cmd, "add") == 0) {

    if (argc != 4) {
      fprintf(stderr, argc > 4 ? ERR_TOO_MANY : ERR_TOO_FEW);
      return 1;
    }

    const char *word = argv[2];
    const char *meaning = argv[3];

    bool success = add_entry(word, meaning);

    if (!success) {
      fprintf(stderr, ERR_ADD_FAILURE);
      return 1;
    }

    printf(MSG_ADD_SUCCESS);

  }

  else if (strcmp(cmd, "delete") == 0) {
    if (argc != 3) {
      fprintf(stderr, argc > 3 ? ERR_TOO_MANY : ERR_TOO_FEW);
      return 1;
    }

    const char *word = argv[2];

    bool success = delete_entry(word);

    if (!success) {
      fprintf(stderr, ERR_DELETE_FAILURE);
      return 1;
    }

    printf(MSG_DELETE_SUCCESS);

  }

  else if (strcmp(cmd, "search") == 0) {

    if (argc > 3) {
      fprintf(stderr, ERR_TOO_MANY);
      return 1;
    }
    if (argc < 3) {
      fprintf(stderr, ERR_TOO_FEW);
      return 1;
    }

    const char *word = argv[2];

    Entry *e = search_entry(word);

    if (e == NULL) {
      printf("Unable to find the word.\n");
    } else {
      show_entry(e);
    }

  }

  else {
    show_help();
  }

  return 0;
}
