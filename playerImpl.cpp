#include "playerImpl.hpp"

#include <stdexcept>

#include "ioSpDef.hpp"
#include "arrowItem.hpp"
#include "arrowTrigger.hpp"
#include "wumpus.hpp"



int BasicPlayer::toDirection(int input) const {
  if(input == 'w') {
    return UpDir;
  }
  if(input == 's') {
    return DownDir;
  }
  if(input == 'a') {
    return LeftDir;
  }
  if(input == 'd') {
    return RightDir;
  }
  return NONE;
}

int BasicPlayer::toAction(int input) const {
  if(input == 'f') {
    return FireAction;
  }
  return toDirection(input);
}

std::string BasicPlayer::directionOptions() const {
  return
      "w: Up\n"
      "a: Left\n"
      "s: Down\n"
      "d: Right\n";
}

std::string BasicPlayer::actionOptions() const {
  return
      "w: Move Up\n"
      "a: Move Left\n"
      "s: Move Down\n"
      "d: Move Right\n"
      "f: Fire an Arrow\n";
}

GameUpdate::pointer BasicPlayer::getTurnAction(const int input) {
  GameUpdate::pointer update;
  if(input == 'h') {
    update = new GameUpdate(GameUpdate::DisplayText,
        actionOptions() + "x: Exit the Game\n");
    update.append(new GameUpdate(GameUpdate::GetPlayerInput,
        this, turnActionID));
  }
  else if(notNone(toDirection(input))) {
    // W/A/S/D = move player
    update = new GameUpdate(GameUpdate::MoveObject, this,
        "You can't move in that direction! Please try again.",
        toDirection(input));
  }
  else if(input == ' ') {
    update = getFireAction();
  }

  if(!update) {
    update = new GameUpdate(GameUpdate::DisplayText,
        "Invalid action. Please try again.");
    update.append(new GameUpdate(GameUpdate::GetPlayerInput,
        this, turnActionID));
  }
  return update;
}

GameUpdate::pointer BasicPlayer::getDirectionAction(const int input,
    Trigger* const other) {
  GameUpdate::pointer update;
  bool reprompt = true;
  if(input == 'h') {
    update.append(new GameUpdate(GameUpdate::DisplayText,
        directionOptions() + "x: Exit the Game\n"));
    reprompt = true;
  }
  else if(notNone(toDirection(input))) {
    // W/A/S/D = move player
    if(other) {
      other->setDirection(toDirection(input));
    }
    else {
      moveDir = toDirection(input);
    }
    reprompt = false;
  }
  else {
    update = new GameUpdate(GameUpdate::DisplayText,
        "Invalid action. Please try again.");
    reprompt = true;
  }

  if(reprompt) {
    update.append(new GameUpdate(GameUpdate::GetPlayerInput,
        this, promptTriggerDirectionID));
  }
  return update;
}

GameUpdate::pointer BasicPlayer::getFireAction() {
  constexpr int arrowFireDist = 3;

  Trigger* arrow = new ArrowTrigger(room);
  
  // Check to make sure the player has arrows
  GameUpdate::pointer update = new GameUpdate(GameUpdate::ItemConditionalExact,
      this, ArrowItem::itemName, 0);
  update.append(new GameUpdate(GameUpdate::DisplayText,
      "You don't have any arrows!"));
  update.append(new GameUpdate(GameUpdate::PromptTurnAction, this));
  update.append(new GameUpdate(GameUpdate::EndConditional));
  
  // Display the choice of direction prompt
  update.append(new GameUpdate(GameUpdate::DisplayText,
      "What direction would you like to fire in?"));
  
  // Prompt the player for the direction to fire the arrow
  update.append(new GameUpdate(GameUpdate::GetPlayerInput, this,
      promptTriggerDirectionID, 0));
  update.append(new GameUpdate(GameUpdate::HandlePlayerInput, this,
      arrow, promptTriggerDirectionID));

  // Remove one from the player's arrow count
  update.append(new GameUpdate(GameUpdate::RemoveItem, this,
      ArrowItem::itemName, 1));
  
  // Move the arrow the correct number of spaces
  for(int i = 0; i < arrowFireDist; ++i) {
    update.append(new GameUpdate(GameUpdate::MoveObject, arrow));
  }

  // Trigger the wumpus before removing the arrow
  update.append(new GameUpdate(GameUpdate::TriggerEvent, arrow,
      Wumpus::eventName));

  // Make sure to delete the arrow
  update.append(new GameUpdate(GameUpdate::DestroyObject, arrow,
      GameUpdate::WhatToDelete::TargetTrigger));

  return update;
}


BasicPlayer::BasicPlayer() : Player() {}

std::string BasicPlayer::name() const {
  return "";
}

char BasicPlayer::character(int) const {
  if(state == LostGame) {
    // Shouldn't display the player on the board if they've lost
    return (char)NONE;
  }
  return '*';
}

GameUpdate::pointer BasicPlayer::handleInput(const Item* info, Trigger* other) {
  if(info->name() == turnActionID) {
    return getTurnAction(info->getCount());
  }
  else if(info->name() == promptTriggerDirectionID) {
    return getDirectionAction(info->getCount(), other);
  }
  throw std::runtime_error("Player HandleInput not implemented for input "
      "type: " + info->name());
}

