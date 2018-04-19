#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>

#include"dex_header.h"

#define DEBUG 1
#define MSG(x...) fprintf(stderr, x)

void show(void *, int);

int header_parser(uint8_t *, header_item *);
int map_parser(uint8_t *, map_list *, int);
int string_id_parser(uint8_t *, string_id_item *, int);

int main(int argc, char **argv)
{
  int fd;
  int * file;
  struct stat sb;
  int offset;

  header_item header;
  map_list maps;

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
  map_parser(file, &maps, header.map_off);

  return 0;
}

void show(void *ptr, int size)
{
  int i;
  for(i = 0; i < size; i++)
    i%16==0 ? printf("\n%02x", *((uint8_t *) ptr + i)) :
              printf("  %02x", *((uint8_t *) ptr + i));

  printf("\n");
}

int header_parser(uint8_t * file, header_item * header)
{
  memmove(header, file, 0x70);

  if(DEBUG)
    show(header, 0x70);

  return 0x70;
}

int map_parser(uint8_t * file, map_list * maps, int offset)
{
  int i;
  memmove(&(maps->size), file+offset, sizeof(uint32_t));

  offset += sizeof(uint32_t);

  maps->list = (map_item *) malloc(sizeof(map_item) * maps->size);
  memmove(maps->list, file+offset, sizeof(map_item) * maps->size);

  offset += sizeof(map_item) * maps->size;

  if(DEBUG)
    for(i = 0; i < maps->size; i++)
    {
      printf("list[%d]", i);
      show(maps->list + i,sizeof(map_item));
      printf("\n");
    }

  return offset;
}

int string_id_parser(uint8_t * file, string_id_i
