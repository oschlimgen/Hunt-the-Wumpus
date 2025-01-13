#include "multiplayer.hpp"

#include "ioSpDef.hpp"


PlayerOne::PlayerOne() : Player() {}

std::string PlayerOne::name() const {
  return "Player 1";
}

char PlayerOne::character(int) const {
  if(state == LostGame) {
    // Shouldn't display the player on the board if they've lost
    return (char)NONE;
  }
  return '*';
}

int PlayerOne::toDirection(int input) const {
  if(input == 'w') {
    return UpDir;
  }
  if(input == 's') {
    return DownDir;
  }
  if(input == 'd') {
    return RightDir;
  }
  if(input == 'a') {
    return LeftDir;
  }
  return NONE;
}

int PlayerOne::toAction(int input) const {
  if(input == ' ') {
    return FireAction;
  }
  return toDirection(input);
}

std::string PlayerOne::directionOptions() const {
  return
      "w: Up\n"
      "a: Left\n"
      "s: Down\n"
      "d: Right\n";
}

std::string PlayerOne::actionOptions() const {
  return
      "w: Move Up\n"
      "a: Move Left\n"
      "s: Move Down\n"
      "d: Move Right\n"
      "space: Fire an Arrow\n";
}




PlayerTwo::PlayerTwo() : Player() {}

std::string PlayerTwo::name() const {
  return "Player 2";
}

char PlayerTwo::character(int) const {
  if(state == LostGame) {
    // Shouldn't display the player on the board if they've lost
    return (char)NONE;
  }
  return '~';
}

int PlayerTwo::toDirection(int input) const {
  if(input == KEY_UP) {
    return UpDir;
  }
  if(input == KEY_DOWN) {
    return DownDir;
  }
  if(input == KEY_RIGHT) {
    return RightDir;
  }
  if(input == KEY_LEFT) {
    return LeftDir;
  }
  return NONE;
}

int PlayerTwo::toAction(int input) const {
  if(input == ' ') {
    return FireAction;
  }
  return toDirection(input);
}

std::string PlayerTwo::directionOptions() const {
  return
      "up-arrow: Up\n"
      "left-arrow: Left\n"
      "down-arrow: Down\n"
      "right-arrow: Right\n";
}

std::string PlayerTwo::actionOptions() const {
  return
      "up-arrow: Move Up\n"
      "left-arrow: Move Left\n"
      "down-arrow: Move Down\n"
      "right-arrow: Move Right\n"
      "space: Fire an Arrow\n";
}



std::vector<Player*> getPlayers(const GameSetup* setup) {
  PlayerOne* first = new PlayerOne();
  PlayerTwo* second = new PlayerTwo();

  std::vector<Player*> players = {first, second};
  return players;
}
