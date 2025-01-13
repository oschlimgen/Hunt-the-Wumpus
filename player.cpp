#include "player.hpp"


Player::Player() : Trigger{NONE}, state(NONE) {}
Player::Player(const RoomPos& room) : Trigger(room), state(NONE) {}

Player::~Player() {
  for(Item* i : items) {
    delete i;
  }
}

Trigger* Player::clone() const {
  return nullptr;
}

Player* Player::player() {
  return this;
}

int Player::getState() const {
  return state;
}
void Player::setState(const int curState) {
  state = curState;
}

void Player::addItem(Item*& item) {
  for(Item* i : items) {
    if(i->name() == item->name()) {
      i->updateCount(item->getCount());
      delete item;
      item = nullptr;
      return;
    }
  }
  // Item wasn't in list
  items.push_back(item);
  item = nullptr;
}

Item* Player::getItem(const std::string& name) const {
  for(Item* i : items) {
    if(i->name() == name) {
      return i;
    }
  }
  return nullptr;
}

std::string Player::getItems() const {
  std::string itemList;
  for(Item* i : items) {
    itemList += i->name() + ": " + std::to_string(i->getCount()) + "\n";
  }
  return itemList;
}

int Player::toDirection(int input) const {
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

int Player::toAction(int input) const {
  if(input == 'f') {
    return FireAction;
  }
  return toDirection(input);
}

std::string Player::directionOptions() const {
  return
      "w: Up\n"
      "a: Left\n"
      "s: Down\n"
      "d: Right\n";
}

std::string Player::actionOptions() const {
  return
      "w: Move Up\n"
      "a: Move Left\n"
      "s: Move Down\n"
      "d: Move Right\n"
      "f: Fire an Arrow\n";
}

GameUpdate::pointer Player::turnUpdate(Player* const active, const bool round) {
  return nullptr;
}

GameUpdate::pointer Player::triggerUpdate(Trigger* const trigger) {
  return nullptr;
}

