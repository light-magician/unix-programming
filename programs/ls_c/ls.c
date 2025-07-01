// This program is a copy of 'ls'
//
// compile with:
// clang -o ls_c.o ls.c
//
// run with:
// ./ls_c.o

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  DIR *dp;
  struct dirent *dirp;
  char *path;

  if (argc == 1) {
    path = ".";
  } else if (argc == 2) {
    path = argv[1];
  }

  dp = opendir(path);
  if (dp == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  while ((dirp = readdir(dp)) != NULL)
    puts(dirp->d_name);

  closedir(dp);
  return 0;
}
