/**
 * This file contains the outer shell of a daemon process.
 *
 * compile and execute the daemon with:
 * clang -o ghost.exe daemon.c
 * ./ghost.exe
 * (named ghost for fun, its a hidden program ...)
 *
 * Find the process:
 * ps aux | grep ghost
 *
 * root        19  0.6  0.0   2060   516 ?        S    19:58   0:00 ./ghost.exe
 * root        22  0.0  0.0   3080  1356 pts/1    S+   19:59   0:00 grep ghost
 *
 * Kill using the process ID
 * kill [PID]
 * (kill 19 in the above case)
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int daemonize(void) {
  // mem space for process id
  pid_t pid;
  // fork off parent process
  pid = fork();
  if (pid < 0) {
    return -1;
  }
  // exit parent process
  if (pid > 0) {
    exit(0);
  }
  // create new session
  if (setsid() < 0) {
    return -1;
  }
  // double-fork to ensure a controlling terminal
  // cannot be reaquired
  pid = fork();
  if (pid < 0) {
    return -1;
  }
  // exit the first child
  if (pid > 0) {
    exit(0);
  }
  // change working dir to root
  chdir("/");
  // close all open file descriptors
  int fd;
  for (fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
    close(fd);
  }
  // reopen stdin, stdout, stderr to /dev/null
  open("/dev/null", O_RDWR); // stdin
  dup(0);                    // stdout
  dup(0);                    // stderr

  return 0;
}

int main(void) {
  if (daemonize() < 0) {
    exit(1);
  }
  while (1) {
    sleep(30);
  }
  return 0;
}
