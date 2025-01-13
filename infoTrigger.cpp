#include "infoTrigger.hpp"


InfoTrigger::InfoTrigger(const std::string& text) : Trigger(NONE),
  text(text) {}
InfoTrigger::InfoTrigger(const RoomPos& room) : Trigger(room) {}

Trigger* InfoTrigger::clone() const {
  return new InfoTrigger(*this);
}

std::string InfoTrigger::name() const {
  return text;
}
