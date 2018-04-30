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

typedef struct
  map_list
{
  uint32_t size;
  map_item *list;
} map_list;

typedef struct
  string_item
{
  uint32_t string_data_off;
  uint32_t utf16_size;
  uint8_t * data;
} string_item;

typedef struct
  type_id_item
{
  uint32_t descriptor_idx;
} type_id_item;

typedef struct
  proto_id_item
{
  uint32_t shorty_idx;
  uint32_t return_type_idx;
  uint32_t parameters_off;
} proto_id_item;

