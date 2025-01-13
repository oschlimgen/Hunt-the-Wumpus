#include "arrowEvent.hpp"

#include "arrowItem.hpp"


ArrowEvent::ArrowEvent(Event* const defaultEvent) : Event(defaultEvent) {}

Event* ArrowEvent::clone() const {
  return new ArrowEvent(*this);
}

char ArrowEvent::character() const {
  return 'A';
}

std::string ArrowEvent::name() const {
  return eventName;
}

Item* ArrowEvent::getItem() {
  return new ArrowItem;
}

GameUpdate::pointer ArrowEvent::triggerUpdate(Trigger* const trigger) {
  if(trigger->player()) {
    GameUpdate::pointer update = new GameUpdate(GameUpdate::PickupItem,
        trigger->player(), this);
    update.append(new GameUpdate(GameUpdate::DestroyObject, this));

    update.append(new GameUpdate(GameUpdate::WaitTurnCycle));
    update.append(new GameUpdate(GameUpdate::DisplayText,
        "You discovered an arrow!"));
    update.append(new GameUpdate(GameUpdate::EndWait));
    return update;
  }
  return nullptr;
}
