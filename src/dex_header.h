#include <stdint.h>

typedef struct
  header_item
{
  uint8_t magic[8];
  uint32_t checksum;
  uint8_t signature[20];
  uint32_t file_size;
  uint32_t header_size;
  uint32_t endian_tag;
  
  uint32_t link_size;
  uint32_t link_off;
  
  uint32_t map_off;
  
  uint32_t string_ids_size;
  uint32_t string_ids_off;
  
  uint32_t type_ids_size;
  uint32_t type_ids_off;
  
  uint32_t proto_ids_size;
  uint32_t proto_ids_off;
  
  uint32_t field_ids_size;
  uint32_t field_ids_off;
  
  uint32_t method_ids_size;
  uint32_t method_ids_off;
  
  uint32_t class_defs_size;
  uint32_t class_defs_off;

  uint32_t data_size;
  uint32_t data_off;
} header_item;

typedef struct
  map_item
{
  uint16_t type;
  uint16_t unused;
  uint32_t size;
  uint32_t offset;
} map_item;
/*
#define TYPE_HEADER_ITEM 0x0000
#define TYPE_STRING_ID_ITEM 0x0001
#define TYPE_TYPE_ID_ITEM 0x0002
#define TYPE_PROTO_ID_ITEM 0x0003
#define TYPE_FIELD_ID_ITEM 0x0004
*/
typedef struct
  map_list
{
  uint32_t size;
  map_item *list;
} map_list;

typedef struct
  string_id_item
{
  uint32_t string_data_off;
} string_id_item;

typedef struct
  string_data_item
{
  leb utf16_size;
  uint8_t * data;
} string_data_item;

