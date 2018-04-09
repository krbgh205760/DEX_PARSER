#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "dex_header.h"

#define MSG(x...) fprintf(stderr, x)

header_item * header;
string_id_item ** string_ids;
map_list * maps;

int header_parser(int);
int map_parser(int);
int string_ids_parser(int);
int string_data_parser(int);

int main(int argc, char *argv[]) {
  int fd;

  if(argc != 2)
  {
   MSG("Usage : dex [filename]\n");
    exit(1);
  }

  if((fd = open(argv[1], O_RDONLY)) == -1)
  {
    MSG("No Such File! : %s\n", argv[1]);
    exit(1);
  }

  header_parser(fd);

  lseek(fd, header->map_off, SEEK_SET);
  map_parser(fd);

  lseek(fd, header->string_ids_off, SEEK_SET);
  string_ids_parser(fd);
  string_data_parser(fd);

  return 0;
}

int header_parser(int fd)
{
  /***************************************************/
  //have to change sbrk
  /***************************************************/
  header = (header_item *) malloc(sizeof(header_item));

  if(read(fd, header, sizeof(header_item)) != sizeof(header_item))
  {
    MSG("Error for reading header!\n");
    exit(1);
  }

  return 0;
}

int map_parser(int fd)
{
  maps = (map_list *) malloc(sizeof(map_list));

  if(read(fd, &maps->size, sizeof(uint32_t)) != sizeof(uint32_t))
  {
    MSG("Error for reading map size!\n");
    exit(1);
  }

  maps->list = (map_item *) malloc(sizeof(map_item) * maps->size);

  if(read(fd, maps->list, sizeof(maps->list)) != sizeof(maps->list))
  {
    MSG("Error for reading map list!\n");
    exit(1);
  }

  return 0;
}

int string_ids_parser(int fd)
{
  string_ids = (string_id_item *) malloc(sizeof(string_id_item) * header->string_ids_size);
  if(read(fd, string_ids, sizeof(string_ids)) != sizeof(string_ids))
  {
    MSG("Error for reading string!\n");
    exit(1);
  }

  return 0;
}

int string_data_parser(int fd)
{
  int i;
  for(i=0; i < header->string_ids_size; i++)
  {
  }
  return 0;
}

int magic_number()
{
  return 0;
}

int checksum()
{
  return 0;
}

int signature()
{
  return 0;
}

int file_size(int fd)
{
  return 0;
}

int header_size()
{
  return 0;
}

int lebtoint(leb in)
{
  int result;

  return result;
}

leb inttoleb(int in)
{
  leb result;
  return result;
}
