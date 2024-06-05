#include "kernel/types.h"
#include "kernel/param.h"
#include "user.h"

int
is_blank(char ch)
{
  return ch == '\0' || ch == '\t';
}

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    const char* usage_str = "Usage: xargs command args1[, args2, ...]";
    printf("%s\n", usage_str);
    exit(-1);
  }

  // xargs' arguments
  char *xargs_argv[MAXARG];
  int xargs_argc = 0;
  for (int i = 1; i < argc; ++i) {
    xargs_argv[xargs_argc++] = argv[i];
  }

  // buffer
  char buf[2048];
  char *str = buf;
  char *p = buf;
  char ch;
  int blank_num = 0;
  int non_blank_num = 0;

  while (read(0, &ch, sizeof(char)) > 0) {
    if (is_blank(ch)) {
      ++blank_num;
    } else {
      if (non_blank_num == 1) {
        str = p;
      }

      blank_num = 0;
      ++non_blank_num;
    }

    if (blank_num >= 1) {
      if (non_blank_num > 0 && blank_num == 1) {
        // a WORD end
        *(++p) = '\0';
        xargs_argv[xargs_argc++] = str;

        blank_num = non_blank_num = 0;
      }

      continue;
    }

    if (ch =='\n') {
      // newline
      *(++p) = '\0';
      xargs_argv[xargs_argc++] = str;
      xargs_argv[xargs_argc++] = 0;

      if (fork() == 0) {
        exec(xargs_argv[0], xargs_argv);
      }
      xargs_argc = argc - 1;
    } else {
      *(++p) = ch;
    }

  }

  exit(0);
}
