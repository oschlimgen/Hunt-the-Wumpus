#include "playerImpl.hpp"

#include "ioSpDef.hpp"


BasicPlayer::BasicPlayer() : Player() {}

std::string BasicPlayer::name() const {
  return "Player";
}

char BasicPlayer::character(int) const {
  if(state == LostGame) {
    // Shouldn't display the player on the board if they've lost
    return (char)NONE;
  }
  return '*';
}
