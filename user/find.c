#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void
find(char* const root, const char* path);

int
main(int argc, char *argv[])
{

  if(argc <= 1){
    printf("No path given!\n");
    exit(0);
  }
  else if (argc <= 2) {
    printf("No file name given!\n");
    exit(0);
  }

  find(argv[1], argv[2]);

  return 0;
}

void
find(char* const root, const char* path)
{

  int fd;

  if((fd = open(root, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", root);
    return;
  }

  struct stat st;

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", root);
    close(fd);
    return;
  }

  char buf[512], *p;
  struct dirent de;

  switch(st.type){
    case T_DEVICE:
    case T_FILE:
      if (strcmp(fmtname(root), path) == 0) {
        printf("%s\n", root);
      }

      break;

    case T_DIR:
      if(strlen(root) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: root too long\n");
        break;
      }
      strcpy(buf, root);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          printf("find: cannot stat %s\n", buf);
          continue;
        }
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
          continue;
        }
        find(buf, path);
      }
      break;

    default:{
      printf("Unknown file type: %d\n", st.type);
    }
  }

  close(fd);
}