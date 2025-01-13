#include "gameSetup.hpp"


GameSetup::GameSetup(const Event* const defaultEvent) :
    defaultEvent(defaultEvent) {}

GameSetup::~GameSetup() {
  delete defaultEvent;
  for(Event* ev : events) {
    delete ev;
  }
  for(Player* pl : players) {
    delete pl;
  }
}


int GameSetup::getSetupVar(const int type) const {
  if(type == GameMode) {
    return gameMode;
  } else if(type == Height) {
    return dimensions.at(0);
  } else if(type == Width) {
    return dimensions.at(1);
  }
  return NONE;
}

Event* GameSetup::getDefaultEvent() const {
  return defaultEvent->clone();
}

const std::vector<Event*>& GameSetup::getEvents() const {
  return events;
}
void GameSetup::addEvent(Event*& ev) {
  events.push_back(ev);
  ev = nullptr;
}
void GameSetup::clearEvents() {
  events.clear();
}

const std::vector<Player*>& GameSetup::getPlayers() const {
  return players;
}
void GameSetup::addPlayer(Player*& pl) {
  players.push_back(pl);
  pl = nullptr;
}
void GameSetup::clearPlayers() {
  players.clear();
}

