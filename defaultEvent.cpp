#include "defaultEvent.hpp"


DefaultEvent::DefaultEvent() : Event(nullptr, NONE) {}
DefaultEvent::~DefaultEvent() {
  defaultEvent = nullptr;
}

Event* DefaultEvent::clone() const {
  return new DefaultEvent;
}

char DefaultEvent::character() const {
  return (char)NONE;
}
char DefaultEvent::character(int mode) const {
  if(defaultEvent && mode == DebugMode) {
    return defaultEvent->character();
  }
  return (char)NONE;
}

std::string DefaultEvent::name() const {
  return eventName;
}

GameUpdate::pointer DefaultEvent::triggerUpdate(Trigger* trigger) {
  // No default update necessary, but function is present for overriding
  return nullptr;
}
