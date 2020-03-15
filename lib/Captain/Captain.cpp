#include "Captain.h"

Captain::Captain() {}

Captain::Captain(Navigator *navigatorp):
  navigator(navigatorp) {}

void Captain::voyage(position *route, int length) {
  for (int i=0; i < length; i++) {
    navigator->sailto(route[i]);    
  }
}
