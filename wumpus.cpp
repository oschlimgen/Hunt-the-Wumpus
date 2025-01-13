#include "wumpus.hpp"

#include "arrowTrigger.hpp"


Wumpus::Wumpus(Event* const defaultEvent) : Event(defaultEvent) {}

Event* Wumpus::clone() const {
  return new Wumpus(*this);
}

char Wumpus::character() const {
  return 'W';
}

std::string Wumpus::name() const {
  return eventName;
}

std::string Wumpus::getPercept(int) const {
  return "You smell a terrible stench.";
}

GameUpdate::pointer Wumpus::triggerUpdate(Trigger* const trigger) {
  if(trigger->player()) {
    if(trigger->getLocation() == room) {
      return new GameUpdate(GameUpdate::LoseGame,
          "The Wumpus angrily eats the foolish adventurer "
          "that entered its room.");
    }

  } else if(trigger->name() == ArrowTrigger::triggerName) {
    if(trigger->getLocation() == room) {
      return new GameUpdate(GameUpdate::WinGame,
          "You have killed the Wumpus!!");
    } else {
      return new GameUpdate(GameUpdate::MoveObjectRandom, this, 1);
    }
  }
  return nullptr;
}
