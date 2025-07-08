/*
 * ============================================================
 * program
 * ============================================================
 * This program demonstrates the fork function
 * and shows how changes to variables in a child process
 * do not affect the value of variables in the parent process.
 *
 * Every process is assigned a unique process ID.
 * When a process terminates, old process IDs can be reused
 * for new processes. Most UNIX systems attempt to delay
 * that reuse though.
 *
 * PID 0 is often a scheduler process often known as swapper.
 * PID 1 is often the init process and is invoked by the kernel
 * at the end of the bootstrap procedure.
 * PID 2 might be the pagedaemon, responsible paging in virtual
 * memory system.
 *
 * ============================================================
 * fork
 * ============================================================
 *
 * An existing process can create a new one by calling fork.
 * The new function is called the child.
 * The fork function is called once but returns twice.
 * The return value in the child process is 0, and the
 * return value in the parent is the PID of the new child.
 * Processes can have only one parent, but parents can have many
 * child processes. There is no function that gets PID's of
 * child processes from a parent but getppid() will get the
 * parent PID of a child process.
 *
 * Both the parent and the child continue executing with
 * with instruction that follows call to fork.
 * The child is a copy of the parent, this includes the child
 * receiving a copy of the parent's data space, heap, and stack.
 * It is a copy, however. The parent and child do not share these
 * portions of memory. The parent and child do share the text
 * segment, however.
 *
 * Modern implementations of fork() don't perform a complete copy
 * of parents memory as fork() is often followed by an exec.
 * The copy will be performed whenever the parent or child attempts
 * to modify those memory portions, and thus it is only performed
 * if needed. This copy is generally a page in virtual memory.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int globvar = 6; // global vars are stored in data segment for initialized
                 // globals (not heap or stack)
char buf[] =
    "a write to stdout\n"; // this will be shared between parent and child

void err_sys(const char *msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(1);
}

int main(void) {
  int var; // automatic variable on stack
  pid_t pid;

  var = 88;
  // STDOUT_FILENO is defined in <unistd.h> and
  // is a macro that represents the file descriptor
  // for standard output, typically with a value of 1
  if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
    err_sys("write error");
  }
  printf("before fork\n"); // don't flush stdout
  // fork
  if ((pid = fork()) < 0) {
    // when process is forked, both the parent and the child
    // go through the remaining instructions, which means both
    // will go through our if else statements and both will print
    // at the end of execution
    err_sys("fork error");
  } else if (pid == 0) { // if result of fork is 0 this is the child process
    globvar++; // modify variables of the child process to show diff from parent
    var++;
  } else {
    sleep(2); // if its a PID int > 0 its the new PID of the child but that is
              // returned to parent
  }
  // parent process will end up with lower glob and var
  printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
  exit(0);
}
