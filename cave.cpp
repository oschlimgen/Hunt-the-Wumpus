#include "cave.hpp"

Cave::Cave() : gameMode(NONE) {}

void Cave::setGameMode(const int mode) {
  gameMode = mode;
}
