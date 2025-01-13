#include "arrowTrigger.hpp"



ArrowTrigger::ArrowTrigger(const RoomPos& pos) : Trigger(pos) {}

ArrowTrigger::ArrowTrigger(const RoomPos& pos, int dir) : Trigger(pos) {
  moveDir = dir;
}

Trigger* ArrowTrigger::clone() const {
  return new ArrowTrigger(*this);
}

std::string ArrowTrigger::name() const {
  return triggerName;
}
