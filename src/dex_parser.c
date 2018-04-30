#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>

#include"dex_parser.h"

#define DISPLAY 0
#define MSG(x...) fprintf(stderr, x)

void show(void *, int);

int header_parser(uint8_t *, header_item *);
int map_parser(uint8_t *, map_list *, int);
int string_id_parser(uint8_t *, string_item *, int);
int string_data_parser(uint8_t *, string_item *, int);
int string_parser(uint8_t *, string_item *, int, int);
int readuleb(uint8_t *, uint32_t *, int);
int type_id_parser(uint8_t *, type_id_item *, int, int);
int proto_id_parser(uint8_t *, proto_id_item *, int, int);

int main(int argc, char **argv)
{
  int fd;
  int * file;
  struct stat sb;
  int offset;

  header_item header;
  map_list maps;
  string_item * strings;
  type_id_item * type_ids;
  proto_id_item * proto_ids;

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

  strings = (string_item *) malloc(sizeof(string_item) * header.string_ids_size);
  string_parser(file, strings, header.string_ids_off, header.string_ids_size); 

  type_ids = (type_id_item *) malloc(sizeof(type_id_item) * header.type_ids_size);
  type_id_parser(file, type_ids, header.type_ids_off, header.type_ids_size);

  proto_ids = (proto_id_item *) malloc(sizeof(proto_id_item) * header.proto_ids_size);
  proto_id_parser(file, proto_ids, header.proto_ids_off, header.proto_ids_size);

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

  if(DISPLAY)
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

  if(DISPLAY)
    for(i = 0; i < maps->size; i++)
    {
      printf("\nlist[%d]", i);
      show(maps->list + i,sizeof(map_item));
    }

  return offset;
}

int string_parser(uint8_t * file, string_item * strings, int offset, int size)
{
  int i, j;

  for(i = 0; i < size; i++)
  {
    string_id_parser(file, strings + i, offset + i * sizeof(uint32_t));
  }
  for(i = 0; i < size; i++)
  {
    string_data_parser(file, strings + i, strings[i].string_data_off);
  }

  if(DISPLAY)
  {
    printf("string size : %d\n", size);
    for(i = 0; i < size; i++)
    {
      printf("\nstring[%d].off : %d\n", i, strings[i].string_data_off);
      printf("\nstring[%d].size : %d", i, strings[i].utf16_size + 1);
      printf("\nstring[%d].data : ", i);
      for(j = 0; j <= strings[i].utf16_size; j++)
        printf("%02x  ", strings[i].data[j]);
      printf("\n");
    }
  }
  
  return offset;
}

int type_id_parser(uint8_t * file, type_id_item * types, int offset, int size)
{
  int i = 0;

  for(i = 0; i < size; i++)
  {
    memmove(types + i, file + offset + i * sizeof(type_id_item), sizeof(type_id_item));
  }
 
  if(DISPLAY)
  {
    printf("type size : %d\n", size);
    for(i = 0; i < size; i++)
    {
      printf("\ntype_ids[%d] : %d", i, types[i]);
    }
  }

  return offset;
}

int proto_id_parser(uint8_t * file, proto_id_item * proto_ids, int offset, int size)
{
  int i = 0;

  for(i = 0; i < size; i++)
  {
    memmove(proto_ids + i, file + offset + i * sizeof(proto_id_item), sizeof(proto_id_item));
  }
 
  //if(DISPLAY)
  {
    printf("proto_ids_size : %d\n", size);
    for(i = 0; i < size; i++)
    {
      printf("\nproto_ids[%d] : %d", i, proto_ids[i].shorty_idx);
    }
    printf("\n");
  }

  return offset;
}

int string_id_parser(uint8_t * file, string_item * string, int offset)
{
  memmove(string, file+offset, sizeof(uint32_t));
  offset += sizeof(uint32_t);
  return offset;
}

int string_data_parser(uint8_t * file, string_item * string, int offset)
{
  int length = 0;

  length = readuleb(file, &string->utf16_size, offset);
  length *= sizeof(uint8_t);
  offset += length;

  string->data = (uint8_t *) malloc(sizeof(uint8_t) * (string->utf16_size + 1));

  memmove(string->data, file + offset, string->utf16_size + 1);

  return offset;
}

int readuleb(uint8_t * file, uint32_t * ptr, int offset)
{
  static int iter= 0;
  int length = 1;
  int8_t * tmp = file + offset;

  uint32_t result = *(tmp++);
  int cur;


  if(result > 0x7f)
  {
    length++;
    cur = *(tmp++);
    result = (result & 0x7f) | ((cur & 0x7f) << 7);
    if(cur > 0x7f) {
      length++;
      cur = *(tmp++);
      result |= (cur & 0x7f) << 14;
      if (cur > 0x7f) {
        length++;
        cur = *(tmp++);
        result |= (cur & 0x7f) << 21;
        if (cur > 0x7f) {
          length++;
          cur = *(tmp++);
          result |= cur << 28;
        }
      }
    }
  }
  *ptr = result;

  return length;
}

/*
u2 dexGetUtf16FromUtf8(const char** pUtf8Ptr)
{
    unsigned int one, two, three;

    one = *(*pUtf8Ptr)++;
    if ((one & 0x80) != 0) {
        two = *(*pUtf8Ptr)++;
        if ((one & 0x20) != 0) {
            three = *(*pUtf8Ptr)++;
            return ((one & 0x0f) << 12) |
                   ((two & 0x3f) << 6) |
                   (three & 0x3f);
        } else {
            return ((one & 0x1f) << 6) |
                   (two & 0x3f);
        }
    } else {
        return one;
    }
}
*/
