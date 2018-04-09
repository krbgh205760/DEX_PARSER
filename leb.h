#ifndef LEB_H_
#define LEB_H_

#include <stdint.h>

typedef struct
  leb
{
  uint8_t value;
  struct leb * next;
} leb;

int lebtoint(leb);
leb inttoleb(int);

int ulebtoint(leb);
leb inttouleb(int);

#endif
