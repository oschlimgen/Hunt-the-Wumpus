#include "goldEvent.hpp"

#include "goldItem.hpp"


GoldEvent::GoldEvent(Event* const defaultEvent) : Event(defaultEvent) {}

Event* GoldEvent::clone() const {
  return new GoldEvent(*this);
}

char GoldEvent::character() const {
  return 'G';
}

std::string GoldEvent::name() const {
  return eventName;
}

std::string GoldEvent::getPercept(int) const {
  return "You see something shimmer nearby.";
}

Item* GoldEvent::getItem() {
  return new GoldItem;
}

GameUpdate::pointer GoldEvent::triggerUpdate(Trigger* const trigger) {
  if(trigger->player()) {
    GameUpdate::pointer update = new GameUpdate(GameUpdate::PickupItem,
        trigger->player(), this);
    update.append(new GameUpdate(GameUpdate::DestroyObject, this));
    
    update.append(new GameUpdate(GameUpdate::WaitTurnCycle));
    update.append(new GameUpdate(GameUpdate::DisplayText,
        "You stumbled upon the gold of the Wumpus!!"));
    update.append(new GameUpdate(GameUpdate::EndWait));
    return update;
  }
  return nullptr;
}
