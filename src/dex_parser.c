#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>

#define MSG(x..) fprintf(stderr, x)

int main(int argc, char **argv)
{
  int fd;

  if(argc != 2)
  {
    MSG("Usage : dex [filename]\n");
    exit(1);
  }

  if((fd = open(argv[1], O_RDONLY)) < 0)
  {
    MSG("File Open Error!\n");
    exit(1);
  }


  return 0;
}
