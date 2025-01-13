#include "trigger.hpp"


Trigger::Trigger(const RoomPos& room) : room(room), moveDir(NONE), enabled(true) {}

Trigger::~Trigger() = default;

Player* Trigger::player() {
  return nullptr;
}

bool Trigger::getEnabled() const {
  return enabled;
}
void Trigger::setEnabled(bool state) {
  enabled = state;
}

const RoomPos& Trigger::getLocation() const {
  return room;
}
void Trigger::setLocation(const RoomPos& loc) {
  room = loc;
}

int Trigger::getDirection() const {
  return moveDir;
}
void Trigger::setDirection(int dir) {
  moveDir = dir;
}
