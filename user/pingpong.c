#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int buff_size = 10;

int
main(int argc, char *argv[])
{
    int p[2];
    char *buff[buff_size];
    pipe(p);

    if (fork() == 0) {
        read(p[0], buff, 1);
        printf("%d: received ping\n", getpid());

        write(p[1], "b", 1);
        exit(0);
    }
    else {
        write(p[1], "b", 1);

        read(p[0], buff, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    }

  exit(0);
}
