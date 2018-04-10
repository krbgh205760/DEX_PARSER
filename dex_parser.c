#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "dex_header.h"

#define MSG(x...) fprintf(stderr, x)

header_item * header;
string_id_item * string_ids;
string_data_item * string_data;
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
//  string_data_parser(fd);

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

  if(saferead(fd, &maps->size, sizeof(uint32_t)))
  {
    MSG("Error for reading map size!\n");
    exit(1);
  }

  maps->list = (map_item *) malloc(sizeof(map_item) * maps->size);

  if(saferead(fd, maps->list, sizeof(map_item) * maps->size))
  {
    MSG("Error for reading map list!\n");
    exit(1);
  }
  printf("map_list->size : %d\n", maps->size);

  return 0;
}

int string_ids_parser(int fd)
{
  string_ids = (string_id_item *) malloc(sizeof(string_id_item) * header->string_ids_size);
  if(saferead(fd, string_ids, sizeof(string_id_item) * header->string_ids_size))
  {
    MSG("Error for reading string!\n");
    exit(1);
  }
  int i;
  for(i=0;i<20;i++)
    printf("string[%d]\t:\t%d\n",i,string_ids[i].string_data_off);

  return 0;
}

int string_data_parser(int fd)
{
  int i;
  for(i=0; i < header->string_ids_size; i++)
  {
    fd = readleb(fd, &(string_data[i].utf16_size));
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

int readleb(int fd, leb* val)
{
  int8_t test_bit = 0x80;
  leb * tmp = NULL;

  if(saferead(fd, val, sizeof(int8_t)))
  {
    MSG("Error in Reading String!\n");
  }

  tmp = val;
  
  while(val->size | test_bit)
  {

  }
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

int saferead(int fd, void * addr, size_t size)
{
  if(mmap(fd, addr, size) == size)
    return 0;
  return 1;
}
