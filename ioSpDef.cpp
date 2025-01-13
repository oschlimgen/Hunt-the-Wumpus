#include "ioSpDef.hpp"

int getchEsc() {
  int key = getch();
  if(key == GETCH_ESC) {
    key = GETCH_ESC * getch();
  }
  return key;
}
