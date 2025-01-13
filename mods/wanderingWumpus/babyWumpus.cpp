#include "babyWumpus.hpp"

#include "player.hpp"
#include "arrowTrigger.hpp"


BabyWumpus::BabyWumpus(Event* const defaultEvent, const RoomPos& room) :
    Event(defaultEvent, room) {}

Event* BabyWumpus::clone() const {
  return new BabyWumpus(*this);
}

char BabyWumpus::character() const {
  return 'w';
}

std::string BabyWumpus::name() const {
  return eventName;
}

std::string BabyWumpus::getPercept(int) const {
  return "You feel like you're being watched.";
}

GameUpdate::pointer BabyWumpus::triggerUpdate(Trigger* trigger) {
  Player* player = trigger->player();
  if(player) {
    double p = (double)rand() / (double)RAND_MAX;
    if(p < killChance) {
      return new GameUpdate(GameUpdate::LoseGame,
          "You've been eaten by a baby Wumpus!");
    }

    GameUpdate::pointer message = new GameUpdate(GameUpdate::WaitTurnCycle, 1);
    message.append(new GameUpdate(GameUpdate::RefreshBoardDisplay));
    message.append(new GameUpdate(GameUpdate::DisplayText, "\n" +
        player->name() + ": You've been attacked by a baby Wumpus! "
        "Choose a direction to run."));
    message.append(new GameUpdate(GameUpdate::PromptDirection, player));

    // Move player to a random adjacent room as they flee
    message.append(new GameUpdate(GameUpdate::MoveObject, player, NONE));
    message.append(new GameUpdate(GameUpdate::ForceUpdateEnd));
    message.append(new GameUpdate(GameUpdate::EndWait));
    return message;

  } else if(trigger->name() == ArrowTrigger::triggerName) {
    if(trigger->getLocation() == room) {
      GameUpdate::pointer update; // Set enabled of arrow to false
      update.append(new GameUpdate(GameUpdate::SetObjectEnabled,
          trigger, false));
      update.append(new GameUpdate(GameUpdate::DestroyObject, this));
      update.append(new GameUpdate(GameUpdate::WaitTurnCycle));
      update.append(new GameUpdate(GameUpdate::DisplayText,
          "You killed a baby Wumpus."));
      update.append(new GameUpdate(GameUpdate::EndWait));
      return update;

    } else {
      // info = 3: the BabyWumpus should be moved to an adjacent empty room
      return new GameUpdate(GameUpdate::MoveObjectRandom, this, 3);
    }
  }
  return nullptr;
}
