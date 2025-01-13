#include "room.hpp"

#include <random>


int Room::randIndex(const int size) {
  return rand() % size;
}


Room::Room() = default;

Room::Room(const RoomPos& pos) : pos(pos), events() {}

Room::Room(const Room& other) : pos(other.pos), events(other.events.size()) {
  auto it = events.begin();
  auto oit = other.events.begin();
  for(; it != events.end(); ++it, ++oit) {
    *it = (*oit)->clone();
  }
}

Room& Room::operator=(const Room& other) {
  if(this != &other) {
    // Free memory
    for(Event*& ev : events) {
      delete ev;
    }

    // Assign members
    pos = other.pos;
    events.resize(other.events.size());
    
    // Deep copy pointers
    auto it = events.begin();
    auto oit = other.events.begin();
    for(; it != events.end(); ++it, ++oit) {
      *it = (*oit)->clone();
    }
  }
  return *this;
}

Room::~Room() {
  for(Event* ev : events) {
    delete ev;
  }
}

const RoomPos& Room::getLocation() const {
  return pos;
}
void Room::setLocation(const RoomPos& loc) {
  pos = loc;
}

bool Room::empty() const {
  // The room is empty if it contains only the default event
  return (events.size() <= 1 && players.size() == 0);
}

Event* Room::getEvent(const std::string& eventName) {
  for(Event* ev : events) {
    if(ev->name() == eventName) {
      return ev;
    }
  }
  return nullptr;
}

void Room::addEvent(Event* const event) {
  events.push_back(event);
}
void Room::removeEvent(Event* const event) {
  for(auto it = events.begin(); it != events.end(); ++it) {
    if(*it == event) {
      events.erase(it);
      return;
    }
  }
}

void Room::addPlayer(Player* const player) {
  players.push_back(player);
}
void Room::removePlayer(Player* const player) {
  for(auto it = players.begin(); it != players.end(); ++it) {
    if(*it == player) {
      players.erase(it);
      return;
    }
  }
}

char Room::getChar(const int mode) const {
  for(const Player* pl : players) {
    char c = pl->character(mode);
    if(c != (char)NONE) {
      return c;
    }
  }
  for(const Event* ev : events) {
    char c = ev->character(mode);
    if(c != (char)NONE) {
      return c;
    }
  }
  return (char)NONE;
}

std::string Room::getPercepts(const int mode) const {
  std::vector<Event*> randomOrderEvents = events;
  for(int i = 0; i < randomOrderEvents.size(); ++i) {
    int index = randIndex(randomOrderEvents.size());
    std::swap(randomOrderEvents.at(i), randomOrderEvents.at(index));
  }

  std::string percepts;
  for(const Event* ev : randomOrderEvents) {
    percepts += ev->getPercept(mode);
  }
  return percepts;
}

GameUpdate::pointer Room::triggerTurnUpdate(Player* const active,
    const bool round) {
  GameUpdate::pointer update = nullptr;
  for(Player* pl : players) {
    GameUpdate::pointer p = pl->turnUpdate(active, round);
    update.append(p);
  }
  for(Event* ev : events) {
    GameUpdate::pointer p = ev->turnUpdate(active, round); // Get the gameUpdate
    update.append(p); // Add it to the end of update
  }
  return update;
}

GameUpdate::pointer Room::triggerEvents(Trigger* const trigger) {
  GameUpdate::pointer update = nullptr;
  for(Player* pl : players) {
    GameUpdate::pointer p = pl->triggerUpdate(trigger);
    update.append(p);
  }
  for(Event* ev : events) {
    GameUpdate::pointer p = ev->triggerUpdate(trigger); // Get the gameUpdate
    update.append(p); // Add it to the end of update
  }
  return update;
}
