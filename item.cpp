#include "item.hpp"


Item::Item(const int count) : itemCount(count) {}

int Item::getCount() const {
  return itemCount;
}

void Item::updateCount(const int toAdd) {
  itemCount += toAdd;
  if(itemCount < 0) {
    itemCount = 0;
  }
}

bool Item::isVisible(const int gameMode) const {
  return true;
}
