#include "batSwarm.hpp"

#include "player.hpp"


BatSwarm::BatSwarm(Event* const defaultEvent) : Event(defaultEvent) {}

Event* BatSwarm::clone() const {
  return new BatSwarm(*this);
}

char BatSwarm::character() const {
  return 'S';
}

std::string BatSwarm::name() const {
  return eventName;
}

std::string BatSwarm::getPercept(int) const {
  return "You hear wings flapping.";
}

GameUpdate::pointer BatSwarm::triggerUpdate(Trigger* const trigger) {
  Player* player = trigger->player();
  if(player) {
    std::string msg;
    if(player->name().empty()) {
      msg = "\n";
    }
    else {
      msg = "\n" + player->name() + ": ";
    }
    msg += "You've been swarmed by bats and must escape! "
        "Choose a direction to move.\n";

    // info = 1 indicates the normal turn prompt should be disabled
    GameUpdate::pointer update = new GameUpdate(GameUpdate::WaitTurnCycle, 1);
    update.append(new GameUpdate(GameUpdate::RefreshBoardDisplay));
    update.append(new GameUpdate(GameUpdate::DisplayText, msg));
    update.append(new GameUpdate(GameUpdate::GetPlayerInput, player,
        Player::promptTriggerDirectionID));

    // info = 2 indicates the player should be moved in a random direction
    update.append(new GameUpdate(GameUpdate::MoveObjectRandom, player, 2));
    update.append(new GameUpdate(GameUpdate::ForceUpdateEnd));
    update.append(new GameUpdate(GameUpdate::EndWait));
    return update;
  }
  return nullptr;
}
