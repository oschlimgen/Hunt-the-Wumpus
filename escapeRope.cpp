#include "escapeRope.hpp"

#include "goldItem.hpp"


EscapeRope::EscapeRope(Event* defaultEvent) : Event(defaultEvent) {}

Event* EscapeRope::clone() const {
  return new EscapeRope(*this);
}

char EscapeRope::character() const {
  return 'R';
}

std::string EscapeRope::name() const {
  return eventName;
}

GameUpdate::pointer EscapeRope::triggerUpdate(Trigger* const trigger) {
  if(trigger->player()) {
    GameUpdate::pointer update = new GameUpdate(GameUpdate::ItemConditional,
        trigger, GoldItem::itemName);
    
    update.append(new GameUpdate(GameUpdate::WinGame,
        "You have escaped with the gold of the fearsome Wumpus!"));
    update.append(new GameUpdate(GameUpdate::EndConditional));
    return update;
  }
  return nullptr;
}
