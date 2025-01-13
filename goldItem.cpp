#include "goldItem.hpp"

GoldItem::GoldItem() : Item(1) {}
GoldItem::GoldItem(const int count) : Item(count) {}

std::string GoldItem::name() const {
  return itemName;
}
