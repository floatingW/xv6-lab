#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    const char* hint = "Give me a time!\n";
    write(1, hint, strlen(hint));
    exit(-1);
  }

  sleep(atoi(argv[1]));
  exit(0);
}
