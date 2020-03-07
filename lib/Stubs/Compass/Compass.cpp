#include "Compass.h"

Compass::Compass():response_list{0},list_size(1),current_position(0) {}

uangle Compass::bearing() {
   uangle result = response_list[current_position];
   if (current_position + 1 < list_size) current_position++;
   return result;
}

void Compass::set_bearings(uangle *bearings, int size) {
  response_list = bearings;
  list_size = size;
  current_position = 0;
}
