#include "caveImpl.hpp"

#include <stdexcept>

#include "ioSpDef.hpp"
#include "arrowItem.hpp"
#include "arrowTrigger.hpp"
#include "wumpus.hpp"

#define EXIT_CODE 0



Room& BasicCave::getRoom(const RoomPos& pos) {
  return cave.at(pos.row).at(pos.col);
}
const Room& BasicCave::getRoom(const RoomPos& pos) const {
  return cave.at(pos.row).at(pos.col);
}

int BasicCave::getAction() {
  int action = getchEsc();
  action = tolower(action);

  if(action == 'x') {
    return EXIT_CODE;
  }

  return action;
}

RoomPos BasicCave::addDirection(const RoomPos& room,
    const int direction) const {
  RoomPos adjacent = room;
  if(direction == UpDir && adjacent.row > 0) {
    adjacent.row--;
  } else if(direction == DownDir && adjacent.row < height - 1) {
    adjacent.row++;
  } else if(direction == LeftDir && adjacent.col > 0) {
    adjacent.col--;
  } else if(direction == RightDir && adjacent.col < width - 1) {
    adjacent.col++;
  }
  return adjacent;
}

RoomPos BasicCave::chooseRoom() {
  int room = (int)(((double)rand() / RAND_MAX) * (height * width));
  int count = 0;
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      if(cave.at(i).at(j).empty()) {
        if(room == count) {
          return RoomPos(i, j);
        }
        count++;
      }
    }
  }
  return RoomPos(0,0);
}

RoomPos BasicCave::chooseEmptyRoom() {
  int emptyCount = 0;
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      if(cave.at(i).at(j).empty()) {
        emptyCount++;
      }
    }
  }

  int room = (int)(((double)rand() / RAND_MAX) * emptyCount);
  emptyCount = 0;
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      if(cave.at(i).at(j).empty()) {
        if(room == emptyCount) {
          return RoomPos(i, j);
        }
        emptyCount++;
      }
    }
  }
  return RoomPos(0,0);
}

int BasicCave::chooseDirection(const RoomPos& pos) {
  int limitedDirection = NONE;
  if(pos.row == 0) {
    limitedDirection = UpDir;
  } else if(pos.row == height - 1) {
    limitedDirection = DownDir;
  } else if(pos.col == 0) {
    limitedDirection = LeftDir;
  } else if(pos.col == width - 1) {
    limitedDirection = RightDir;
  }

  int options = 4 - (int)notNone(limitedDirection);
  int direction = (int)(((double)rand() / RAND_MAX) * options);
  if(notNone(limitedDirection) && direction >= limitedDirection) {
    direction++;
  }
  return direction;
}

int BasicCave::chooseEmptyDirection(const RoomPos& pos) {
  constexpr int numDirs = 4;

  int limitedCount = 0;
  std::vector<bool> limitedDirections(numDirs, false);
  for(int i = 0; i < numDirs; ++i) {
    RoomPos adjacent = addDirection(pos, i);
    // If there isn't a room in the direction or it isn't empty
    if(adjacent == pos || !getRoom(adjacent).empty()) {
      limitedDirections[i] = true;
      limitedCount++;
    }
  }

  int options = numDirs - limitedCount;
  if(options == 0) {
    return NONE;
  }
  int direction = (int)(((double)rand() / RAND_MAX) * options);

  for(int i = 0; i < numDirs; ++i) {
    if(limitedDirections[i] && direction >= i) {
      direction++;
    }
  }
  return direction;
}



std::string BasicCave::getItems(Player* player) const {
  return player->getItems();
}

std::string BasicCave::getBoard() const {
  std::string board;
  std::string row_border = "-";
  for (int i = 0; i < width; ++i) {
    row_border += "----";
  }

  board += row_border + "\n";
  for (int i = 0; i < height; ++i) {
    board += "| ";
    for (int j = 0; j < width; ++j) {
      // Get the character representing the room if there is one
      char c = cave.at(i).at(j).getChar(gameMode);

      // If none of the calls have yielded a character result, insert a space.
      if(c == (char)NONE) {
        c = ' ';
      }
      board += c;

      board += " | ";
    }
    board += "\n";
    board += row_border + "\n";
  }
  return board;
  
  //example output (when finished): 
  // -----------------
  // | P | G | P |   |
  // -----------------
  // |   | W |   | S |
  // -----------------
  // |   |   |   | S |
  // -----------------
  // | * |   |   |   |
  // -----------------
}

std::string BasicCave::getPercepts(Player* const player) const {
  std::string percepts;
  RoomPos room = player->getLocation();

  // For each direction
  for(int i = 0; i < 4; ++i) {
    RoomPos adjacent = addDirection(room, i);
    if(adjacent != room) {
      std::string p = getRoom(adjacent).getPercepts(gameMode);
      if(!p.empty()) {
        percepts += p + "\n";
      }
    }
  }
  return percepts;
}


GameUpdate::pointer BasicCave::actionUpdate(Player* const player,
    const int actionInput) {
  GameUpdate::pointer update = nullptr;
  if(actionInput == 'h') {
    update = new GameUpdate(GameUpdate::DisplayText,
        player->actionOptions() + "x: Exit the Game\n");
    update.append(new GameUpdate(GameUpdate::PromptPlayerAction, player));
    
  } else if(notNone(player->toDirection(actionInput))) {
    // W/A/S/D = move player
    update = new GameUpdate(GameUpdate::MoveObject, player,
        "You can't move in that direction! Please try again.",
        player->toDirection(actionInput));

  } else if(actionInput == ' ') {
    update = fireArrow(player);
  }

  if(!update) {
    update = new GameUpdate(GameUpdate::DisplayText,
        "Invalid action. Please try again.");
    update.append(new GameUpdate(GameUpdate::PromptPlayerAction, player));
  }
  return update;
}

GameUpdate::pointer BasicCave::fireArrow(Player* const active) {
  Trigger* arrow = new ArrowTrigger(active->getLocation());
  
  // Check to make sure the player has arrows
  GameUpdate::pointer update = new GameUpdate(GameUpdate::ItemConditional,
      active, ArrowItem::itemName, 0);
  update.append(new GameUpdate(GameUpdate::DisplayText,
      "You don't have any arrows!")); // String not getting freed
  update.append(new GameUpdate(GameUpdate::PromptPlayerAction, active));
  update.append(new GameUpdate(GameUpdate::EndConditional));
  
  // Display the choice of direction prompt
  update.append(new GameUpdate(GameUpdate::DisplayText,
      "What direction would you like to fire in?"));
  
  // Prompt the player for the direction to fire the arrow
  update.append(new GameUpdate(GameUpdate::PromptDirection, active, arrow));

  // Remove one from the player's arrow count
  update.append(new GameUpdate(GameUpdate::RemoveItem, active,
      ArrowItem::itemName));
  
  // Move the arrow the correct number of spaces
  for(int i = 0; i < arrowFireDist; ++i) {
    update.append(new GameUpdate(GameUpdate::MoveObject, arrow)); // not getting freed
  }

  // Trigger the wumpus before removing the arrow
  update.append(new GameUpdate(GameUpdate::TriggerEvent, arrow,
      Wumpus::eventName));

  // Make sure to delete the arrow
  update.append(new GameUpdate(GameUpdate::DestroyObject, arrow,
      GameUpdate::WhatToDelete::TargetTrigger));

  return update;
}


GameUpdate::pointer BasicCave::promptForPlayerAction(const GameUpdate& update) {
  if(playerActionEnabled) {
    // Ask player for their action
    int action = getAction();
    if(action == EXIT_CODE) {
      // Quit the game (player entered 'x')
      GameUpdate::pointer addUpdate = new GameUpdate(GameUpdate::ForceGameEnd);
      return addUpdate;
    }
    
    // Restart the update list with update(s) based on the player action.
    GameUpdate::pointer addUpdate = actionUpdate(update.getPlayer(), action);
    addUpdate.append(new GameUpdate(GameUpdate::ForceUpdateEnd));
    return addUpdate;
  }
  return nullptr;
}

GameUpdate::pointer BasicCave::handleDirectionInput(const GameUpdate& update,
    const int action) {
  int direction = update.getPlayer()->toDirection(action);

  GameUpdate::pointer addUpdate = nullptr;
  if(notNone(direction)) {
    if(update.hasTrigger()) {
      update.getTrigger()->setDirection(direction);
    } else {
      update.getPlayer()->setDirection(direction);
    }
    // No need to re-prompt player, return nullptr

  } else {
    // Reprompt for direction
    addUpdate = new GameUpdate(GameUpdate::DisplayText,
        "Please enter a valid direction.");
    addUpdate.append(new GameUpdate(update));
    
  }
  return addUpdate;
}

GameUpdate::pointer BasicCave::promptForDirection(const GameUpdate& update) {
  if(!update.hasPlayer()) {
    throw std::runtime_error("No Player assigned to "
        "PromptDirection GameUpdate.");
  }
  
  int action = getAction();
  if(action == EXIT_CODE) {
    // Quit the game (player entered 'x')
    GameUpdate::pointer addUpdate =  new GameUpdate(GameUpdate::ForceGameEnd);
    addUpdate.append(new GameUpdate(update));
    return addUpdate;
  }
  if(action == 'h') {
    GameUpdate::pointer addUpdate = new GameUpdate(GameUpdate::DisplayText,
        update.getPlayer()->directionOptions() + "x: Exit The Game\n");
    addUpdate.append(new GameUpdate(update));
    return addUpdate;
  }

  return handleDirectionInput(update, action);
}


void BasicCave::createObject(const GameUpdate& update) {
  if(update.hasPlayer()) {
    Player* pl = update.getPlayer();
    getRoom(pl->getLocation()).addPlayer(pl);

  } else if(update.hasEvent()) {
    Event* ev = update.getEvent();
    getRoom(ev->getLocation()).addEvent(ev);

  } else {
    throw std::runtime_error("CreateObject call doesn't have an event "
        "or player to create.");
  }
}


RoomPos BasicCave::moveRandom(const RoomPos& from, const int info) {
  if(info == 0) {
    return chooseRoom();

  } else if(info == 1) {
    return chooseEmptyRoom();

  } else if(info == 2) {
    int direction = chooseDirection(from);
    return addDirection(from, direction);

  } else if(info == 3) {
    int direction = chooseEmptyDirection(from);
    return addDirection(from, direction);

  } else {
    throw std::invalid_argument("Info member of MoveRandom event must be "
        "set to a number 0-3.");
  }
}

GameUpdate::pointer BasicCave::movePlayer(const GameUpdate& update) {
  Player* pl = update.getPlayer();
  RoomPos from = pl->getLocation();
  RoomPos moveTo;

  if(update == GameUpdate::MoveObjectRandom) {
    moveTo = moveRandom(from, update.getInfo());
    
  } else {
    moveTo = update.getRoom();
    if(moveTo.empty()) {
      int direction;
      if(notNone(update.getInfo())) {
        direction = update.getInfo();
      } else {
        direction = pl->getDirection();
      }
      moveTo = addDirection(pl->getLocation(), direction);
    }
  }
  pl->setLocation(moveTo);

  getRoom(from).removePlayer(pl);
  getRoom(moveTo).addPlayer(pl);
  
  GameUpdate::pointer triggered = nullptr;
  if(pl->getEnabled() && from != moveTo) {
    triggered = getRoom(moveTo).triggerEvents(pl);
  } else if(!update.getMessage().empty()) {
    triggered = new GameUpdate(GameUpdate::DisplayText, update.getMessage());
    triggered.append(new GameUpdate(GameUpdate::PromptPlayerAction, pl));
  }
  return triggered;
}

GameUpdate::pointer BasicCave::moveEvent(const GameUpdate& update) {
  Event* ev = update.getEvent();
  RoomPos from = ev->getLocation();
  RoomPos moveTo;
  
  if(update == GameUpdate::MoveObjectRandom) {
    moveTo = moveRandom(from, update.getInfo());
    
  } else {
    moveTo = update.getRoom();
    if(moveTo.empty()) {
      int direction = update.getInfo();
      if(notNone(direction)) {
        moveTo = addDirection(ev->getLocation(), direction);

      } else {
        moveTo = chooseEmptyRoom();
      }
    }
  }
  ev->setLocation(moveTo);

  getRoom(from).removeEvent(ev);
  getRoom(moveTo).addEvent(ev);

  return nullptr;
}

GameUpdate::pointer BasicCave::moveTrigger(const GameUpdate& update) {
  Trigger* tg = update.getTrigger();
  RoomPos from = tg->getLocation();
  RoomPos moveTo;
  
  if(update == GameUpdate::MoveObjectRandom) {
    moveTo = moveRandom(from, update.getInfo());
    
  } else {
    moveTo = update.getRoom();
    if(moveTo.empty()) {
      int direction;
      if(notNone(update.getInfo())) {
        direction = update.getInfo();
      } else {
        direction = tg->getDirection();
      }
      moveTo = addDirection(tg->getLocation(), direction);
    }
  }
  tg->setLocation(moveTo);
  
  GameUpdate::pointer triggered = nullptr;
  if(tg->getEnabled() && from != moveTo) {
    triggered = getRoom(moveTo).triggerEvents(tg);
  }
  return triggered;
}

GameUpdate::pointer BasicCave::moveObject(const GameUpdate& update) {
  GameUpdate::pointer addUpdate;
  if(update.hasPlayer()) {
    addUpdate = movePlayer(update);

  } else if(update.hasEvent()) {
    addUpdate = moveEvent(update);

  } else if(update.hasTrigger()) {
    addUpdate = moveTrigger(update);
  } else {
    throw std::runtime_error("MoveObject call doesn't have a player, "
        "event or trigger to move.");
  }
  return addUpdate;
}

void BasicCave::destroyObject(const GameUpdate& update) {
  if(update.getPlayer()) {
    Player* pl = update.getPlayer();
    getRoom(pl->getLocation()).removePlayer(pl);
    if(update.hasPlayer()) {
      delete pl;
    }

  } else if(update.getEvent()) {
    Event* ev = update.getEvent();
    getRoom(ev->getLocation()).removeEvent(ev);
    if(update.hasEvent()) {
      delete ev;
    }

  } else if(update.getTrigger()) {
    if(update.hasTrigger()) {
      delete update.getTrigger();
    }

  } else {
    throw std::runtime_error("DestroyObject call doesn't have an event "
        "or player to remove.");
  }
}


GameUpdate::pointer BasicCave::evaluateItemConditional(
    const GameUpdate& update) {
  Item* item = update.getPlayer()->getItem(update.getMessage());
  bool checkPassed = false;
  if(item) {
    if(notNone(update.getInfo())) {
      if(update.getInfo() == 0) {
        checkPassed = item->getCount() == 0;
      } else {
        checkPassed = item->getCount() >= update.getInfo();
      }
    } else {
      checkPassed = *item;
    }
  } else if(update.getInfo() == 0) {
    checkPassed = true;
  }

  if(!checkPassed) {
    return new GameUpdate(GameUpdate::BeginConditional);
  }
  return nullptr;
}

GameUpdate::pointer BasicCave::triggerEventWithName(const GameUpdate& update) {
  GameUpdate::pointer addUpdate = nullptr;
  std::string name = update.getMessage();
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      Event* ev = cave.at(i).at(j).getEvent(name);
      if(ev) {
        addUpdate.append(ev->triggerUpdate(update.getTrigger()));
      }
    }
  }
  return addUpdate;
}



BasicCave::BasicCave(const GameSetup* const setup) : Cave() {
  height = setup->getSetupVar(GameSetup::Height);
  width = setup->getSetupVar(GameSetup::Width);

  cave.resize(height, std::vector<Room>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      cave.at(i).at(j).setLocation({i, j});
      cave.at(i).at(j).addEvent(setup->getDefaultEvent());
    }
  }

  for(Event* ev : setup->getEvents()) {
    RoomPos pos = ev->getLocation();
    getRoom(pos).addEvent(ev);
  }
}



std::string BasicCave::turnDisplay(Player* const active) {
  std::string output;
  if(active) {
    output += getItems(active);
  }
  output += getBoard();
  if(active) {
    output += getPercepts(active);
  }
  return output;
}

GameUpdate::pointer BasicCave::getTurnUpdate(Player* const active,
    const bool round) {
  playerActionEnabled = true;

  GameUpdate::pointer update;
  for(int i = 0; i < height; ++i) {
    for(int j = 0; j < width; ++j) {
      GameUpdate::pointer p = cave.at(i).at(j).triggerTurnUpdate(active, round);
      update.append(p);
    }
  }
  return update;
}

GameUpdate::pointer BasicCave::updateState(const GameUpdate& update) {
  GameUpdate::pointer addUpdate = nullptr;
  if(update == GameUpdate::PromptPlayerAction) {
    addUpdate = promptForPlayerAction(update);

  } else if(update == GameUpdate::SetPlayerActionEnabled) {
    if(update.getInfo() == 1) {
      playerActionEnabled = true;
    } else {
      playerActionEnabled = false;
    }

  } else if(update == GameUpdate::PromptDirection) {
    addUpdate = promptForDirection(update);

  } else if(update == GameUpdate::CreateObject) {
    createObject(update);

  } else if(update == GameUpdate::MoveObject ||
      update == GameUpdate::MoveObjectRandom) {
    addUpdate = moveObject(update);

  } else if(update == GameUpdate::DestroyObject) {
    destroyObject(update);

  } else if(update == GameUpdate::PickupItem) {
    Item* item = update.getEvent()->getItem();
    update.getPlayer()->addItem(item);

  } else if(update == GameUpdate::RemoveItem) {
    Item* item = update.getPlayer()->getItem(update.getMessage());
    if(item) {
      if(isNone(update.getInfo())) {
        item->updateCount(-1);
      } else {
        item->updateCount(-update.getInfo());
      }
    }

  } else if(update == GameUpdate::ItemConditional) {
    addUpdate = evaluateItemConditional(update);

  } else if(update == GameUpdate::SetObjectEnabled) {
    update.getTrigger()->setEnabled((bool)update.getInfo());

  } else if(update == GameUpdate::TriggerEvent) {
    addUpdate = triggerEventWithName(update);

  }
  return addUpdate;
}
