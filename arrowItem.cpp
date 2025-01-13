#include "arrowItem.hpp"


ArrowItem::ArrowItem() : Item(1) {}
ArrowItem::ArrowItem(const int count) : Item(count) {}

std::string ArrowItem::name() const {
  return itemName;
}
