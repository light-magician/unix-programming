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
  /* Call fork and have the parent exit.
   *
   * If daemon was started as a shell command,
   * having the parent terminate makes the
   * shell think the command is finished.
   *
   * Child inherits the process group id of
   * the parent but gets its own process id,
   * so it is guaranteed that the daemon is
   * not a process group leader. This is a
   * prerequisite for the call to setsid later.
   */
  pid = fork();
  if (pid < 0) {
    return -1;
  }
  // exit parent process
  if (pid > 0) {
    exit(0);
  }
  /*
   * Call setsid to create a new session.
   * The process then:
   * - becomes leader of a new session
   * - becomes leader of a new process group
   * - is disassociated from its controlling terminal
   */
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
  /*
   * Change current working directory to root directory.
   * Current working directory inherited from parent could
   * be on a mounted filesystem. Daemons normally exist until
   * the system is rebooted, thus if it stays on a mounted
   * filesystem, the filesystem cannot be unmounted.
   *
   * It wouls also make sense to change to a dir in which
   * the daemon will do all of its work.
   */
  chdir("/");
  /*
   * Umask ensures that any files the daemon creates
   * will have permissions unrestricted by the inherited unmask.
   * umask(0) clears all restrictions - files get exactly the permissions
   * you specify when creating them.
   */
  umask(0);
  /*
   * Unneeded file descriptors should be closed so that
   * any descriptors inherited from parent do not remain open.
   */
  int fd;
  for (fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--) {
    close(fd);
  }
  /*
   * Opening file descriptors 0, 1, 2 to /dev/null
   * ensures that any library routines that try to
   * read from stdin or write to stdout or stderr
   * will have no effect. This puts the process
   * "fully in the background" as there is no way
   * to receive input or for output to be displayed.
   */
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
