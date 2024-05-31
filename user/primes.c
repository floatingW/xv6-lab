#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAKE_PIPE(name) int name[2]; pipe(name);
const int N = 35;

int
child_process(int fd, int prime);

int
main(int argc, char *argv[])
{
  MAKE_PIPE(pipe_fds)

  if (fork() == 0) {
    for (int i = 2; i <= N; ++i) {
      write(pipe_fds[1], &i, sizeof(int));
    }
    exit(0);
  }

  close(pipe_fds[1]);

  int fd = pipe_fds[0];
  int prime;
  while(read(fd, &prime, sizeof(int)) > 0)
  {
    printf("prime %d\n", prime);
    fd = child_process(fd, prime);
  }

  return 0;
}

int
child_process(int fd, int prime){

  MAKE_PIPE(pipe_fds)
  if (fork() == 0) {
    int num;
    while(read(fd, &num, sizeof(int)) > 0)
    {
      if (num % prime != 0) {
        write(pipe_fds[1], &num, sizeof(int));
      }
    }

    exit(0);
  }

  close(fd);
  close(pipe_fds[1]);
  return pipe_fds[0];
}