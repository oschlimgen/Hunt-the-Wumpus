#include "gameUpdate.hpp"

#include "player.hpp"
#include "infoEvent.hpp"
#include "infoTrigger.hpp"


GameUpdate::GameUpdate(const int type) : type(type) {}

GameUpdate::GameUpdate(const int type, const int info) :
    type(type), info(info) {}
GameUpdate::GameUpdate(const int type, Event* const event) :
    type(type), targetEvent(event) {}
GameUpdate::GameUpdate(const int type, Trigger* const trigger) :
    type(type) {
  Player* player = trigger->player();
  if(player) {
    targetPlayer = player;
  } else {
    targetTrigger = trigger;
  }
}
GameUpdate::GameUpdate(const int type, const std::string& message) : type(type),
    targetTrigger(new InfoTrigger(message)),
    toDelete(WhatToDelete::TargetTrigger) {}

GameUpdate::GameUpdate(const int type, const std::string& message,
    const int info) : type(type), info(info),
    targetTrigger(new InfoTrigger(message)),
    toDelete(WhatToDelete::TargetTrigger) {}


GameUpdate::GameUpdate(const int type, Event* const event,
    const int info) : type(type), info(info), targetEvent(event) {}
GameUpdate::GameUpdate(const int type, Event* const event,
    const RoomPos room) : type(type), info(info), targetEvent(event),
    targetTrigger(new InfoTrigger(room)),
    toDelete(WhatToDelete::TargetTrigger) {}
GameUpdate::GameUpdate(const int type, Event* const event,
    const WhatToDelete toDelete) : type(type), targetEvent(event),
    toDelete(toDelete) {}

GameUpdate::GameUpdate(const int type, Trigger* const trigger,
    const int info) : type(type), info(info) {
  Player* player = trigger->player();
  if(player) {
    targetPlayer = player;
  } else {
    targetTrigger = trigger;
  }
}
GameUpdate::GameUpdate(const int type, Trigger* const trigger,
    const RoomPos room) : type(type), targetEvent(new InfoEvent(room)),
    toDelete(WhatToDelete::TargetEvent) {
  Player* player = trigger->player();
  if(player) {
    targetPlayer = player;
  } else {
    targetTrigger = trigger;
  }
}
GameUpdate::GameUpdate(const int type, Trigger* const trigger,
    const WhatToDelete toDelete) : type(type), toDelete(toDelete) {
  Player* player = trigger->player();
  if(player) {
    targetPlayer = player;
  } else {
    targetTrigger = trigger;
  }
}


GameUpdate::GameUpdate(const int type, Player* const player,
    Event* const event) : type(type), targetPlayer(player),
    targetEvent(event) {}
GameUpdate::GameUpdate(const int type, Player* const player,
    Event* const event, const int info) : type(type), targetPlayer(player),
    targetEvent(event), info(info) {}
GameUpdate::GameUpdate(const int type, Player* const player,
    Event* const event, const std::string& message) :
    type(type), targetPlayer(player), targetEvent(event),
    targetTrigger(new InfoTrigger(message)),
    toDelete(WhatToDelete::TargetEvent) {}

GameUpdate::GameUpdate(const int type, Player* const player,
    Trigger* const trigger) : type(type), targetPlayer(player),
    targetTrigger(trigger) {}
GameUpdate::GameUpdate(const int type, Player* const player,
    Trigger* const trigger, const int info) : type(type), info(info),
    targetPlayer(player), targetTrigger(trigger) {}
GameUpdate::GameUpdate(const int type, Player* const player,
    Trigger* const trigger, const std::string& message) :
    type(type), targetPlayer(player), targetEvent(new InfoEvent(message)),
    targetTrigger(trigger), toDelete(WhatToDelete::TargetEvent) {}


GameUpdate::GameUpdate(int type, Event* event, const std::string& message) :
    type(type), targetEvent(event), targetTrigger(new InfoTrigger(message)),
    toDelete(WhatToDelete::TargetTrigger) {}
GameUpdate::GameUpdate(int type, Event* event, const std::string& message,
    int info) : type(type), info(info), targetEvent(event),
    targetTrigger(new InfoTrigger(message)),
    toDelete(WhatToDelete::TargetTrigger) {}

GameUpdate::GameUpdate(int type, Trigger* trigger,
    const std::string& message) : type(type),
    targetEvent(new InfoEvent(message)), toDelete(WhatToDelete::TargetEvent) {
  Player* player = trigger->player();
  if(player) {
    targetPlayer = player;
  } else {
    targetTrigger = trigger;
  }
}
GameUpdate::GameUpdate(int type, Trigger* trigger, const std::string& message,
    int info) : type(type), info(info),
    targetEvent(new InfoEvent(message)), toDelete(WhatToDelete::TargetEvent) {
  Player* player = trigger->player();
  if(player) {
    targetPlayer = player;
  } else {
    targetTrigger = trigger;
  }
}


GameUpdate::GameUpdate(const int type, Player* const player,
    Event* const event, Trigger* const trigger, const int info,
    const WhatToDelete toDelete) : type(type), info(info),
    targetPlayer(player), targetEvent(event), targetTrigger(trigger),
    toDelete(toDelete) {}


GameUpdate::GameUpdate(const GameUpdate& other) : type(other.type),
    info(other.info), targetPlayer(other.targetPlayer),
    targetEvent(other.targetEvent), targetTrigger(other.targetTrigger),
    toDelete(other.toDelete) {
  if(toDelete == WhatToDelete::TargetEvent) {
    targetEvent = other.targetEvent->clone();
  }
  if(toDelete == WhatToDelete::TargetTrigger) {
    targetTrigger = other.targetTrigger->clone();
  }
}

GameUpdate& GameUpdate::operator=(const GameUpdate& other) {
  if(this != &other) {
    if(toDelete == WhatToDelete::TargetEvent) {
      delete targetEvent;
    } else if(toDelete == WhatToDelete::TargetTrigger) {
      delete targetTrigger;
    }

    type = other.type;
    info = other.info;
    targetPlayer = other.targetPlayer;
    targetEvent = other.targetEvent;
    targetTrigger = other.targetTrigger;
    toDelete = other.toDelete;

    if(toDelete == WhatToDelete::TargetEvent) {
      targetEvent = other.targetEvent->clone();
    }
    if(toDelete == WhatToDelete::TargetTrigger) {
      targetTrigger = other.targetTrigger->clone();
    }
  }
  return *this;
}


GameUpdate::~GameUpdate() {
  if(toDelete == WhatToDelete::TargetEvent) {
    delete targetEvent;
  } else if(toDelete == WhatToDelete::TargetTrigger) {
    delete targetTrigger;
  }
}



GameUpdate::operator int() const { return type; }
int GameUpdate::getType() const { return type; }
int GameUpdate::getInfo() const { return info; }
Player* GameUpdate::getPlayer() const { return targetPlayer; }
Event* GameUpdate::getEvent() const { return targetEvent; }
Trigger* GameUpdate::getTrigger() const { return targetTrigger; }


bool GameUpdate::hasPlayer() const {
  return (targetPlayer && toDelete != WhatToDelete::TargetPlayer);
}
bool GameUpdate::hasEvent() const {
  return (targetEvent && toDelete != WhatToDelete::TargetEvent);
}
bool GameUpdate::hasTrigger() const {
  return (targetTrigger && toDelete != WhatToDelete::TargetTrigger);
}


RoomPos GameUpdate::getRoom() const {
  if(toDelete == WhatToDelete::TargetEvent) {
    return targetEvent->getLocation();
  } else if(toDelete == WhatToDelete::TargetTrigger) {
    return targetTrigger->getLocation();
  }
  return NONE;
}
std::string GameUpdate::getMessage() const {
  if(toDelete == WhatToDelete::TargetEvent) {
    return targetEvent->name();
  } else if(toDelete == WhatToDelete::TargetTrigger) {
    return targetTrigger->name();
  }
  return "";
}
