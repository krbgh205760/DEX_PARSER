#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>

#define MSG(x..) fprintf(stderr, x)

void header_parser(int *, header_item *);
void map_parser(int *, header_item *);

int main(int argc, char **argv)
{
  int fd;
  int * file;
  struct stat sb;
  off_t offset, pa_offset;
  size_t length;
  ssize_t s;
  header_item header;

  if(argc != 2)
  {
    MSG("Usage : dex [filename]\n");
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);

  if(fd == -1)
  {
    MSG("File Open Error!\n");
    exit(1);
  }

  if(fstat(fd, &sb) == -1)
  {
    MSG("fstat Error!\n");
    exit(1);
  }

  file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(file == -1)
  {
    MSG("mmap error\n");
    exit(1);
  }

  header_parser(file, &header);


  return 0;
}

void header_parser(int * file, header_item * header)
{
  int i;

  memmove(header, file, 0x70);
  
  for(i=0;i<0x70;i++)
    printf("%02x\n", *header+i);
}
