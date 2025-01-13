#include "wanderingWumpus.hpp"

#include "babyWumpus.hpp"


WanderingWumpus::WanderingWumpus(Event* const defaultEvent) :
    Wumpus(defaultEvent) {}

Event* WanderingWumpus::clone() const {
  return new WanderingWumpus(*this);
}

GameUpdate::pointer WanderingWumpus::turnUpdate(Player* const active,
    const bool round) {
  GameUpdate::pointer update = nullptr;
  if(round) {
    roundCount++;
    if(roundCount == createBabiesAfter) {
      for(int i = 0; i < babiesToCreate; ++i) {
        Event* baby = new BabyWumpus(defaultEvent->clone(), room);
        update.append(new GameUpdate(GameUpdate::CreateObject, baby));
        // Place the baby in a random empty room.
        update.append(new GameUpdate(GameUpdate::MoveObjectRandom, baby, 1));
      }
    } else {
      update.append(new GameUpdate(GameUpdate::MoveObjectRandom, this, 3));
    }
  }
  
  if(roundCount == createBabiesAfter) {
    update.append(new GameUpdate(GameUpdate::DisplayText,
        "Baby Wumpus have been born. Watch Out!"));
  }
  return update;
}


Event* getEvent(const GameSetup* setup) {
  return new WanderingWumpus(setup->getDefaultEvent());
}
