#include "infoItem.hpp"


InfoItem::InfoItem(const std::string& name, const int data) :
    Item(data), infoLabel(name) {}

bool InfoItem::isVisible(const int mode) const {
  return false;
}

std::string InfoItem::name() const {
  return infoLabel;
}

