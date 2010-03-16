#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

#define BINARY "/usr/bin/ctags-exuberant"
#define TIMEOUT (5 * 60)

pid_t pid;
int status;

void alarm_handler(int signum)
{
  kill(pid, SIGKILL);
}

int main (int argc, char * argv[])
{
  pid = fork();
  if (pid == -1)
    {
      perror("wrapper for " BINARY ": cannot fork");
      return 1;
    }
  if (pid == 0)
    {
      execve(BINARY, argv, NULL);
      perror("wrapper for " BINARY ": cannot exec");
      return 1;
    }

  signal(SIGALRM, alarm_handler);
  alarm(TIMEOUT);
  while (waitpid(pid, &status, 0) == EINTR)
    ;
  return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
