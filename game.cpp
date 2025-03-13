#include "game.hpp"

#include <iostream>
#include <string>

#include "ioSpDef.hpp"
#include "infoItem.hpp"



int Game::getAction() {
  int action = getchEsc();
  action = tolower(action);

  if(action == 'x') {
    return GAME_EXIT_CODE;
  }

  return action;
}


int Game::playerCount() const {
  int count = 0;
  for(const Player* pl : players) {
    if(pl->getState() != Player::LostGame) {
      count++;
    }
  }
  return count;
}

Player* Game::activePlayer() {
  return players.at(playing);
}

int Game::getNextPlayer(int pl) {
  do {
    pl++;
    if(pl == players.size()) {
      pl = 0;
    }
  } while(players.at(pl)->getState() == Player::LostGame);
  
  return pl;
}

void Game::swapTurn() {
  if(activePlayer()->getState() == Player::Active) {
    activePlayer()->setState(NONE);
  }

  do {
    playing++;
    if(playing == players.size()) {
      playing = 0;
      roundStart = true;
    }
  } while(activePlayer()->getState() == Player::LostGame);

  activePlayer()->setState(Player::Active);
}



void Game::displayGame() {
  CLEAR_SCREEN;

  Player* pl = activePlayer();
  if(activePlayer()->getState() == Player::LostGame ||
      activePlayer()->getState() == Player::WonGame) {
    pl = nullptr;
  }
  std::string output = cave->turnDisplay(pl);

  print(output);
  boardDisplayed = true;

  // Print any queued TextDisplays
  print(toDisplay);
  toDisplay.clear();
}

bool Game::checkWin() const {
  // Only one player must have won for the win condition to trigger
  for(const Player* pl : players) {
    if(pl->getState() == Player::WonGame) {
      return true;
    }
  }
  return false;
}

bool Game::checkLose() const {
  // All players must have lost for the lose condition to trigger
  for(const Player* pl : players) {
    if(pl->getState() != Player::LostGame) {
      return false;
    }
  }
  return true;
}

bool Game::confirmExit() {
  print("Are you sure you want to exit the game? (y/n): ");
  int response = getAction();

  if(response == 'y') {
    print("y\n\n");
    return true;
  } else {
    print("n\n\n");
    return false;
  }
}

bool Game::passUpdate(GameUpdate::pointer& list, const int updateType) {
  while(list) {
    if(!list.empty()) {
      if(*list == updateType) {
        return true;
      }
    }
    list.moveToNext();
  }
  return false;
}

GameUpdate::pointer Game::getUpdatesUntilType(GameUpdate::pointer& list,
    const int updateType) {
  GameUpdate::pointer passed;
  while(list) {
    if(!list.empty()) {
      // Check if eventType has been found
      if(*list == updateType) {
        return passed;
      }
      // Add the list pointer to passed
      GameUpdate::pointer p = list;
      p.setNext(nullptr);
      passed.append(p);
    }
    list.moveToNext();
  }
  return passed;
}

GameUpdate::pointer Game::getWaitUpdates(GameUpdate::pointer& update) {
  GameUpdate::pointer addUpdate;

  update.moveToNext(); // Skip the WaitForMove or WaitTurnCycle update
  GameUpdate::pointer waitUpdate = getUpdatesUntilType(update,
    GameUpdate::EndWait);
  update.moveToNext(); // Skip the EndWait update

  if(waitUpdate->getInfo() == 1) {
    addUpdate.append(new GameUpdate(GameUpdate::SetPlayerActionEnabled));
  }
  addUpdate.append(waitUpdate);
  return addUpdate;
}

void Game::insertMultiTurn(const GameUpdate::pointer& update, int waitCount) {
  int passedWaits = 0;
  int pl = playing; // Keep track of the associated player
  for(GameUpdate::pointer* it = &multiTurnUpdates; it;
      it = it->nextPointer()) {
    if(!it->empty() && **it == GameUpdate::WaitForMove &&
        (*it)->getPlayer()->getState() != Player::LostGame) {
      passedWaits++;
      pl = getNextPlayer(pl);

      if(passedWaits == waitCount) {
        GameUpdate::pointer* upds = it->nextPointer();
        it->setNext(update);
        it->append(*upds);
        delete upds;
        return;
      }
    }
  }

  // At this point, there weren't enough waits already in multiTurnUpdates
  for(int i = passedWaits; i < waitCount; ++i) {
    pl = getNextPlayer(pl);
    multiTurnUpdates.append(new GameUpdate(GameUpdate::WaitForMove,
        players.at(pl)));
  }
  multiTurnUpdates.append(update);
}



GameUpdate::pointer Game::startTurnUpdate() {
  GameUpdate::pointer update;

  GameUpdate::pointer turnTriggeredUpdate = cave->getTurnUpdate(
      activePlayer(), roundStart);
  update.append(turnTriggeredUpdate);

  // Get everything until the next wait, and move list pointer forward
  if(multiTurnUpdates) {
    multiTurnUpdates.moveToNext(); // Skip the WaitForMove event
    GameUpdate::pointer waitUpdates = getUpdatesUntilType(multiTurnUpdates,
        GameUpdate::WaitForMove);
    update.append(waitUpdates);
  }

  update.append(new GameUpdate(GameUpdate::RefreshBoardDisplay));
  update.append(new GameUpdate(GameUpdate::PromptTurnAction, activePlayer()));
  return update;
}

GameUpdate::pointer Game::resolveUpdate(const GameUpdate& update) {
  GameUpdate::pointer addUpdate = nullptr;
  // Check for cases where the update should be resolved by Game
  if(update == GameUpdate::WinGame) {
    displayGame();
    activePlayer()->setState(Player::WonGame);

    std::string text = update.getMessage();
    if(!text.empty()) {
      print("\n\n" + text + "\n");
      print("\n(Press any key to continue)\n");
      getchEsc();
    }
    addUpdate = new GameUpdate(GameUpdate::ForceGameEnd);
  }
  else if(update == GameUpdate::LoseGame) {
    displayGame();
    activePlayer()->setState(Player::LostGame);

    std::string text = update.getMessage();
    if(!text.empty()) {
      print("\n\n" + text + "\n");
      print("\n(Press any key to continue)\n");
      getchEsc();
    }
  }
  else if(update == GameUpdate::SetGameMode) {
    cave->setGameMode(update.getInfo());
  }
  else if(update == GameUpdate::RefreshBoardDisplay) {
    displayGame();
    boardDisplayed = true;
  }
  else if(update == GameUpdate::DisplayText) {
    std::string text = update.getMessage() + '\n';
    if(boardDisplayed) {
      print(text);
    } else {
      toDisplay += text;
    }
  }
  else if(update == GameUpdate::GetPlayerInput) {
    int input = getAction();
    Item* info = new InfoItem(update.getMessage(), input);
    update.getPlayer()->addItem(info);
    if(update.getInfo() != 0) {
      addUpdate = new GameUpdate(GameUpdate::HandlePlayerInput,
          update.getPlayer(), update.getMessage());
    }
  }
  else {
    addUpdate = cave->updateState(update);
  }
  return addUpdate;
}

bool Game::resolveTurn() {
  // Start the update list with prompting the player for an action.
  GameUpdate::pointer update = startTurnUpdate();
  roundStart = false;
  boardDisplayed = false;

  // As long as there are more updates to resolve, keep resolving them.
  while(update) {
    // Make sure the update isn't empty
    if(update.empty()) {
      update.moveToNext();
      continue;
    }
    if(*update == GameUpdate::ForceGameEnd) {
      // Exit without prompting if game end conditions are met
      if(checkWin() || checkLose()) {
        update.deleteNext();
        return false;
      }
      // Otherwise, confirm the player actually wants to exit
      if(confirmExit()) {
        update.deleteNext();
        return false;
      }

    } else if(*update == GameUpdate::ForceUpdateEnd) {
      update.deleteNext();
      break;

    } else if(*update == GameUpdate::WaitForMove) {
      GameUpdate::pointer waitUpdates = getWaitUpdates(update);
      insertMultiTurn(waitUpdates, 1);

    } else if(*update == GameUpdate::WaitTurnCycle) {
      // Get the updates to execute
      GameUpdate::pointer waitUpdates = getWaitUpdates(update);
      insertMultiTurn(waitUpdates, playerCount());

    } else if(*update == GameUpdate::BeginConditional) {
      bool result = passUpdate(update, GameUpdate::EndConditional);
    }
    
    if(!update.empty()) {
      // Resolve the update if it hasn't been handled yet
      GameUpdate::pointer toAdd = resolveUpdate(*update);
      if(toAdd) {
        update.insertNext(toAdd);
      }
    }

    // Move to resolving next update (will become null if no next update)
    update.moveToNext();
  }

  return !(checkWin() || checkLose());
}



Game::Game(GameSetup* setup, Cave* caveSetup) : cave(caveSetup), playing(0),
    roundStart(true) {
  int gameMode = (GameModeTypes)setup->getSetupVar(GameSetup::GameMode);
  cave->setGameMode(gameMode);

  for(Player* pl : setup->getPlayers()) {
    players.push_back(pl);
    cave->updateState(GameUpdate(GameUpdate::CreateObject, pl));
  }
  players.at(0)->setState(Player::Active);
  setup->clearPlayers();
  
  // Start curses window if using curses library
  CURSES_INIT;
  
  // Pause and wait for key press
  print("Setup Complete. Press any key to begin...");
  getchEsc();
}

Game::~Game() {
  // Close curses window if using curses library
  CURSES_END;

  // Free any outstanding wait updates
  multiTurnUpdates.deleteNext();
  
  // Free memory allocated to players
  for(Player* pl : players) {
    delete pl;
  }
}


void Game::playGame() {
  bool flag = true;
  while (flag) {
    flag = resolveTurn();
    if(flag) {
      swapTurn();
    }
  }

  // Update to the final board state
  displayGame();

  // Display a message about the winner if any
  if(checkWin()) {
    if(players.size() == 1) {
      print("\nYou win!!!\n\n\n");
    } else {
      print("\nPlayer " + std::to_string(playing + 1) + " wins!!!!\n\n\n");
    }
  } else if(checkLose()) {
    std::string plural = "";
    if(players.size() != 1) {
      plural = "s";
    }
    print("\nThe mighty Wumpus has defeated the mere mortal" + plural +
        " who dared venture into its cave.\n\n\n");
  } else {
    print("\nExiting. Please wait...\n\n\n");
  }
}
