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

void Player::addItem(Item* item) {
  for(Item* i : items) {
    if(i->name() == item->name()) {
      // Item is in the list, so add counts
      i->updateCount(item->getCount());
      delete item;
      return;
    }
  }
  // Item wasn't in list
  items.push_back(item);
}

Item* Player::getItem(const std::string& name) {
  for(Item* i : items) {
    if(i->name() == name) {
      return i;
    }
  }
  return nullptr;
}

bool Player::removeItem(const std::string& name) {
  for(std::vector<Item*>::iterator it = items.begin();
      it != items.end(); ++it) {
    if((*it)->name() == name) {
      delete *it;
      items.erase(it);
      return true;
    }
  }
  return false;
}

std::string Player::getItemList(const int mode) const {
  std::string itemList;
  for(Item* i : items) {
    if(i->isVisible(mode)) {
      itemList += i->name() + ": " + std::to_string(i->getCount()) + "\n";
    }
  }
  return itemList;
}

GameUpdate::pointer Player::turnUpdate(Player* const active, const bool round) {
  return nullptr;
}

GameUpdate::pointer Player::triggerUpdate(Trigger* const trigger) {
  return nullptr;
}

