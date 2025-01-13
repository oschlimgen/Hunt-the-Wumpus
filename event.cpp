#include "event.hpp"


Event::Event(Event* defaultEvent) : defaultEvent(defaultEvent), room(NONE) {
  if(defaultEvent) {
    defaultEvent->setEventPointer(this);
  }
}
Event::Event(Event* defaultEvent, const RoomPos& room) : defaultEvent(defaultEvent),
    room(room) {
  if(defaultEvent) {
    defaultEvent->setEventPointer(this);
  }
}

Event::~Event() {
  delete defaultEvent;
}

void Event::setEventPointer(Event* ev) {
  defaultEvent = ev;
}

const RoomPos& Event::getLocation() const {
  return room;
}
void Event::setLocation(const RoomPos& loc) {
  room = loc;
}

char Event::character(int mode) const {
  return defaultEvent->character(mode);
}

std::string Event::getPercept(int mode) const {
  return "";
}

Item* Event::getItem() { return nullptr; }

GameUpdate::pointer Event::turnUpdate(Player* player, bool round) {
  return nullptr;
}
