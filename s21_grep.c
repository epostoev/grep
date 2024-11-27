#define _GNU_SOURCE //getline
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <regex.h>
#include <stdlib.h>

#include "GrepFlags.h"

typedef struct {
    bool s;
} Flags;


Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};

  int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
  Flags flags = {false, false, false, false, false, false};
  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
    switch (currentFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEndl = true;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'E':
        flags.markEndl = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.printNonPrintable = true;
      case 'T':
        flags.tab = true;
        break;
    }
  }
  return flags;
}

void GrepFile(FILE *file, Flags flags, regex_t *preg) {
    (void)flags;
    char *line = 0;
    size_t lengh = 0;
    regmatch_t match;
    while (getline (&line, &lengh, file) > 0) {
        if (!regexec(preg, line, 1, &match, 0)) {
          printf("%s", line);

        }
    }
    free(line);
}

void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  regex_t preg_storange;
  regex_t *preg = &preg_storange;

  for (; pattern != end && pattern[0][0] == '0'; ++pattern)
      ;
  if (pattern == end) {
    fprintf(stderr, "no_pattern\n");
    exit(1);
  }
  if (regcomp(preg, *pattern, 0)) {
    fprintf(stderr, "failed to cimpile regex \n");
    exit(1); 
  }
  for (char **filename = pattern + 1; filename != end; ++filename) {
       if (**filename == '-')
          continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    GrepFile(file, flags, preg);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = {1};
  Grep(argc, argv, flags);
  return 0;
}
