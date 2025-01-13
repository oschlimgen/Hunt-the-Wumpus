#include "item.hpp"


Item::Item(const int count) : itemCount(count) {}

Item::operator bool() {
  return itemCount > 0;
}

int Item::getCount() const {
  return itemCount;
}

void Item::updateCount(const int toAdd) {
  itemCount += toAdd;
  if(itemCount < 0) {
    itemCount = 0;
  }
}
