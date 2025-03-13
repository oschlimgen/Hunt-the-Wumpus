#include "caveImpl.hpp"

#include <stdexcept>
#include <sstream>

#include "arrowItem.hpp"
#include "arrowTrigger.hpp"
#include "wumpus.hpp"



Room& BasicCave::getRoom(const RoomPos& pos) {
  return cave.at(pos.row).at(pos.col);
}
const Room& BasicCave::getRoom(const RoomPos& pos) const {
  return cave.at(pos.row).at(pos.col);
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



std::string BasicCave::joinLines(const std::string& left,
    const std::string& right, int spacing) {
  std::stringstream lss(left);
  std::stringstream rss(right);

  std::string combined;
  std::string lline;
  std::string rline;
  while(true) {
    bool rresult = (bool)std::getline(lss, lline);
    bool lresult = (bool)std::getline(rss, rline);
    if(!rresult && !lresult) {
      break;
    }
    combined += lline + std::string(spacing, ' ') + rline + '\n';
  }
  return combined;
}

std::string BasicCave::getItemList(Player* player) const {
  return player->getItemList(gameMode);
}

std::string BasicCave::getBoard() const {
  std::string board;
  std::string row_border = "-";
  for (int i = 0; i < width; ++i) {
    row_border += "----";
  }
  row_border += " ";

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



GameUpdate::pointer BasicCave::turnPrompt(Player* player) {
  std::string msg;
  if(player->name().empty()) {
    msg += "\nWhat would you like to do? (Press H for options)\n";
  } else {
    msg += "\n" + player->name() + ": What would you like to do? "
      "(Press H for options)\n";
  }

  GameUpdate::pointer update = new GameUpdate(GameUpdate::DisplayText, msg);
  update.append(new GameUpdate(GameUpdate::GetPlayerInput, player,
      Player::turnActionID));
  return update;
}

GameUpdate::pointer BasicCave::handlePlayerInput(const GameUpdate& update) {
  Item* info = update.getPlayer()->getItem(update.getMessage());
  if(!info) {
    throw std::runtime_error("HandlePlayerInput update couldn't find the "
        "player input item with the given name.");
  }

  GameUpdate::pointer addUpdate;
  if(update.hasTrigger()) {
    addUpdate = update.getTrigger()->handleInput(info, update.getPlayer());
  }
  else {
    addUpdate = update.getPlayer()->handleInput(info, nullptr);
  }
  update.getPlayer()->removeItem(info->name());
  return addUpdate;
}

// GameUpdate::pointer BasicCave::actionUpdate(Player* const player,
//     const int actionInput) {
//   GameUpdate::pointer update = nullptr;
//   if(actionInput == 'h') {
//     update = new GameUpdate(GameUpdate::DisplayText,
//         player->actionOptions() + "x: Exit the Game\n");
//     update.append(new GameUpdate(GameUpdate::GetPlayerInput,
//         player, turnActionID));
    
//   } else if(notNone(player->toDirection(actionInput))) {
//     // W/A/S/D = move player
//     update = new GameUpdate(GameUpdate::MoveObject, player,
//         "You can't move in that direction! Please try again.",
//         player->toDirection(actionInput));

//   } else if(actionInput == ' ') {
//     update = player->getFireAction();
//   }

//   if(!update) {
//     update = new GameUpdate(GameUpdate::DisplayText,
//         "Invalid action. Please try again.");
//     update.append(new GameUpdate(GameUpdate::GetPlayerInput,
//         player, turnActionID));
//   }
//   return update;
// }


// GameUpdate::pointer BasicCave::handlePlayerAction(const GameUpdate& update) {
//   if(playerActionEnabled) {
//     // Ask player for their action
//     int action = update.getPlayer()->getItem(update.getMessage())->getCount();
//     if(action == GAME_EXIT_CODE) {
//       // Quit the game (player entered 'x')
//       GameUpdate::pointer addUpdate = new GameUpdate(GameUpdate::ForceGameEnd);
//       return addUpdate;
//     }
    
//     // Restart the update list with update(s) based on the player action.
//     GameUpdate::pointer addUpdate = actionUpdate(update.getPlayer(), action);
//     addUpdate.append(new GameUpdate(GameUpdate::ForceUpdateEnd));
//     return addUpdate;
//   }
//   return nullptr;
// }

// GameUpdate::pointer BasicCave::handleDirectionInput(const GameUpdate& update) {
//   Item* infoItem = update.getPlayer()->getItem(update.getMessage());
//   int direction = NONE;
//   if(infoItem) {
//     direction = update.getPlayer()->toDirection(infoItem->getCount());
//   }

//   GameUpdate::pointer addUpdate = nullptr;
//   if(notNone(direction)) {
//     if(update.hasTrigger()) {
//       update.getTrigger()->setDirection(direction);
//     } else {
//       update.getPlayer()->setDirection(direction);
//     }
//     // No need to re-prompt player, return nullptr
//   }
//   else {
//     // Reprompt for direction
//     addUpdate = new GameUpdate(GameUpdate::DisplayText,
//         "Please enter a valid direction.");
//     addUpdate.append(new GameUpdate(GameUpdate::PromptDirection,
//         update.getPlayer()));
//   }
//   return addUpdate;
// }

// GameUpdate::pointer BasicCave::promptForDirection(const GameUpdate& update) {
//   if(!update.hasPlayer()) {
//     throw std::runtime_error("No Player assigned to "
//         "PromptDirection GameUpdate.");
//   }
//   Player* player = update.getPlayer();
  
//   GameUpdate::pointer addUpdate;
//   addUpdate.append(new GameUpdate(GameUpdate::GetPlayerInput,
//       player, directionPromptID, 0));
  
//   // Check to quit the game (player entered 'x')
//   addUpdate.append(new GameUpdate(GameUpdate::ItemConditionalExact,
//       player, directionPromptID, GAME_EXIT_CODE));
//   addUpdate.append(new GameUpdate(GameUpdate::ForceGameEnd));
//   addUpdate.append(new GameUpdate(GameUpdate::RemoveItem, directionPromptID));
//   addUpdate.append(new GameUpdate(update));
//   addUpdate.append(new GameUpdate(GameUpdate::EndConditional));

//   // Check to display help menu (player entered 'h')
//   addUpdate.append(new GameUpdate(GameUpdate::ItemConditionalExact,
//       player, directionPromptID, 'h'));
//   addUpdate.append(new GameUpdate(GameUpdate::DisplayText,
//         player->directionOptions() + "x: Exit The Game\n"));
//   addUpdate.append(new GameUpdate(GameUpdate::RemoveItem, directionPromptID));
//   addUpdate.append(new GameUpdate(update));
//   addUpdate.append(new GameUpdate(GameUpdate::EndConditional));

//   addUpdate.append(new GameUpdate(GameUpdate::HandlePlayerInput,
//       player, directionPromptID));
//   return addUpdate;
// }


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
    triggered.append(new GameUpdate(GameUpdate::DisplayText,
        update.getMessage()));
    triggered.append(new GameUpdate(GameUpdate::GetPlayerInput,
        pl, Player::turnActionID));
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


void BasicCave::pickupItem(const GameUpdate& update) {
  Item* item = update.getEvent()->getItem();
  update.getPlayer()->addItem(item);
}

void BasicCave::removeItem(const GameUpdate& update) {
  Item* item = update.getPlayer()->getItem(update.getMessage());
  if(item) {
    if(isNone(update.getInfo())) {
      throw std::runtime_error("RemoveItem update must have info value"
          " assigned.");
    }
    else if(update.getInfo() == 0 || item->getCount() <= update.getInfo()) {
      update.getPlayer()->removeItem(item->name());
    }
    else {
      item->updateCount(-update.getInfo());
    }
  }
}


GameUpdate::pointer BasicCave::evaluateItemConditional(
    const GameUpdate& update) {
  bool checkPassed = false;
  Item* item = update.getPlayer()->getItem(update.getMessage());
  int itemCount = 0;
  if(item) {
    itemCount = item->getCount();
  }
  if(isNone(update.getInfo())) {
    throw std::runtime_error("ItemConditional doesn't have info set.");
  }
  else if(update == GameUpdate::ItemConditional) {
    checkPassed = (itemCount >= update.getInfo());
  }
  else if(update == GameUpdate::ItemConditionalExact) {
    checkPassed = (itemCount == update.getInfo());
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
  std::string items;
  if(active) {
    items = "\n" + getItemList(active);
  }
  std::string board = getBoard();

  std::string output = joinLines(board, items, 4);
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
  if(update == GameUpdate::HandlePlayerInput) {
    addUpdate = handlePlayerInput(update);
  }
  if(update == GameUpdate::PromptTurnAction) {
    addUpdate = turnPrompt(update.getPlayer());
  }
  else if(update == GameUpdate::SetPlayerActionEnabled) {
    playerActionEnabled = (update.getInfo() >= 1);
  }
  else if(update == GameUpdate::CreateObject) {
    createObject(update);
  }
  else if(update == GameUpdate::MoveObject ||
      update == GameUpdate::MoveObjectRandom) {
    addUpdate = moveObject(update);
  }
  else if(update == GameUpdate::DestroyObject) {
    destroyObject(update);
  }
  else if(update == GameUpdate::PickupItem) {
    pickupItem(update);
  }
  else if(update == GameUpdate::RemoveItem) {
    removeItem(update);
  }
  else if(update == GameUpdate::ItemConditional ||
      update == GameUpdate::ItemConditionalExact) {
    addUpdate = evaluateItemConditional(update);
  }
  else if(update == GameUpdate::SetObjectEnabled) {
    update.getTrigger()->setEnabled(update.getInfo() >= 1);
  }
  else if(update == GameUpdate::TriggerEvent) {
    addUpdate = triggerEventWithName(update);
  }
  return addUpdate;
}
