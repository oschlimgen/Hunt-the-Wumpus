#include "bottomlessPit.hpp"

#include <random>


BottomlessPit::BottomlessPit(Event* const defaultEvent) : Event(defaultEvent) {}

Event* BottomlessPit::clone() const {
  return new BottomlessPit(*this);
}

char BottomlessPit::character() const {
  return 'P';
}

std::string BottomlessPit::name() const {
  return eventName;
}

std::string BottomlessPit::getPercept(int) const {
  return "You feel a breeze.";
}

GameUpdate::pointer BottomlessPit::triggerUpdate(Trigger* const trigger) {
  if(trigger->player()) {
    double p = (double)rand() / (double)RAND_MAX;
    if(p < fallChance) {
      return new GameUpdate(GameUpdate::LoseGame, "You slipped and fell "
          "into a pit! You still haven't found the bottom...\n"
          "\n...aaaahhhhhhhhh!!!!!!!");
    }

    GameUpdate::pointer message = new GameUpdate(GameUpdate::WaitTurnCycle);
    message.append(new GameUpdate(GameUpdate::DisplayText, "You narrowly "
        "avoided falling into a pit with no discernable bottom."));
    message.append(new GameUpdate(GameUpdate::EndWait));
    return message;
  }
  return nullptr;
}
