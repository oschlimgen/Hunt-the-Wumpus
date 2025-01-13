#include "infoEvent.hpp"


InfoEvent::InfoEvent(const std::string& text) : Event(nullptr),
    text(text) {}
InfoEvent::InfoEvent(const RoomPos& room) : Event(nullptr, room) {}

Event* InfoEvent::clone() const {
  return new InfoEvent(*this);
}

char InfoEvent::character() const {
  return (char)NONE;
}
char InfoEvent::character(int) const {
  return (char)NONE;
}

std::string InfoEvent::name() const {
  return text;
}

GameUpdate::pointer InfoEvent::triggerUpdate(Trigger* trigger) {
  return nullptr;
}
