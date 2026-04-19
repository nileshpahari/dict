#include "cli.h"
#include "hashmap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_TOO_MANY "Too many arguments.\n"
#define ERR_TOO_FEW "Too few arguments.\n"
#define ERR_ADD_FAILURE "Failed to add the word.\n"
#define ERR_DELETE_FAILURE "Failed to delete the word.\n"

#define MSG_ADD_SUCCESS "Word added successfully.\n"
#define MSG_DELETE_SUCCESS "Word deleted successfully.\n"
#define MSG_NOT_FOUND "Word not found.\n"

static int show_help() {
  printf("\nUsage:\n\n"
         "dict add <word> <meaning>\n\n"
         "dict delete <word>\n\n"
         "dict search <word>\n\n"
         "dict help\n\n");
  return 0;
}

static int add_entry(int argc, char **argv, HashMap *hm) {
  if (argc < 4) {
    fprintf(stderr, ERR_TOO_FEW);
    return 1;
  }

  const char *word = argv[2];

  size_t len = 0;
  for (int i = 3; i < argc; i++)
    len += strlen(argv[i]) + 1;

  char *meaning = malloc(len);
  if (!meaning) {
    fprintf(stderr, ERR_ADD_FAILURE);
    return 1;
  }
  meaning[0] = '\0';
  for (int i = 3; i < argc; i++) {
    if (i > 3) strcat(meaning, " ");
    strcat(meaning, argv[i]);
  }

  bool success = hm_add(word, meaning, hm);
  free(meaning);

  if (!success) {
    fprintf(stderr, ERR_ADD_FAILURE);
    return 1;
  }

  printf(MSG_ADD_SUCCESS);
  return 0;
}

static int delete_entry(int argc, char **argv, HashMap *hm) {
  if (argc != 3) {
    fprintf(stderr, argc > 3 ? ERR_TOO_MANY : ERR_TOO_FEW);
    return 1;
  }

  const char *word = argv[2];

  bool success = hm_delete(word, hm);

  if (!success) {
    fprintf(stderr, ERR_DELETE_FAILURE);
    return 1;
  }

  printf(MSG_DELETE_SUCCESS);
  return 0;
}

static int search_entry(int argc, char **argv, HashMap *hm) {
  if (argc != 3) {
    fprintf(stderr, argc > 3 ? ERR_TOO_MANY : ERR_TOO_FEW);
    return 1;
  }

  const char *word = argv[2];

  const char *meaning = hm_get(word, hm);

  if (meaning == NULL) {
    printf(MSG_NOT_FOUND);
  } else {
    printf("\n  Word: %s\n\n"
           "  Meaning: %s\n\n",
           word, meaning);
  }

  return 0;
}

int handle_cmd(HashMap *hm, char *line) {

  char *cmd = strtok(line, " ");
  if (!cmd)
    return 0;

  if (strcmp(cmd, "help") == 0) {
    return show_help();
  }

  if (strcmp(cmd, "add") == 0) {

    char *word = strtok(NULL, " ");
    char *meaning = strtok(NULL, "");

    if (!word || !meaning) {
      printf("\nUsage: add <word> <meaning>\n\n");
      return 0;
    }

    if (!hm_add(word, meaning, hm))
      printf(ERR_ADD_FAILURE);
    else
      printf(MSG_ADD_SUCCESS);

    return 0;
  }

  if (strcmp(cmd, "search") == 0) {

    char *word = strtok(NULL, " ");

    if (!word) {
      printf("Usage: search <word>\n");
      return 0;
    }

    const char *meaning = hm_get(word, hm);

    if (!meaning) {
      printf(MSG_NOT_FOUND);
    } else {
      printf("\n  Word: %s\n\n"
             "  Meaning: %s\n\n",
             word, meaning);
    }

    return 0;
  }

  if (strcmp(cmd, "delete") == 0) {

    char *word = strtok(NULL, " ");

    if (!word) {
      printf("Usage: delete <word>\n");
      return 0;
    }

    if (!hm_delete(word, hm))
      printf(MSG_NOT_FOUND);
    else
      printf(MSG_DELETE_SUCCESS);

    return 0;
  }

  printf("Unknown command.\n");
  return 0;
}

int interactive_mode(HashMap *dict) {
  printf("\nUsage: <command> <argument>\n"
         "Type \"help\" for listing possible commands or \"exit\" for "
         "exiting interactive mode.\n\n");

  char *line = NULL;
  size_t len = 0;
  while (1) {
    printf(">>> ");
    size_t read = getline(&line, &len, stdin);

    if (read == -1) {
      break;
    }

    line[strcspn(line, "\n")] = 0;

    if (strcmp(line, "exit") == 0) {
      break;
    }

    handle_cmd(dict, line);
  }
  free(line);
  return 0;
}

int cmd_mode(int argc, char **argv, HashMap *dict) {
  int status = 0;
  const char *cmd = argv[1];
  if (strcmp(cmd, "help") == 0) {
    status = show_help();
  }

  else if (strcmp(cmd, "add") == 0) {
    status = add_entry(argc, argv, dict);
  }

  else if (strcmp(cmd, "delete") == 0) {
    status = delete_entry(argc, argv, dict);
  }

  else if (strcmp(cmd, "search") == 0) {
    status = search_entry(argc, argv, dict);
  }
  else {
    status = show_help();
  }
  return status;
}
