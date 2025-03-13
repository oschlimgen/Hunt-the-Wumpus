#include "multiplayer.hpp"

#include "ioSpDef.hpp"



PlayerNumber::PlayerNumber(const int playerNum, const char representation) :
    Player(), playerNum(playerNum), representation(representation) {}

std::string PlayerNumber::name() const {
  return "Player " + std::to_string(playerNum);
}

char PlayerNumber::character(int) const {
  if(state == LostGame) {
    // Shouldn't display the player on the board if they've lost
    return (char)NONE;
  }
  return representation;
}

int PlayerNumber::toDirection(int input) const {
  if(input == 'w' || input == KEY_UP) {
    return UpDir;
  }
  if(input == 's' || input == KEY_DOWN) {
    return DownDir;
  }
  if(input == 'd' || input == KEY_RIGHT) {
    return RightDir;
  }
  if(input == 'a' || input == KEY_LEFT) {
    return LeftDir;
  }
  return NONE;
}

int PlayerNumber::toAction(int input) const {
  if(input == ' ') {
    return FireAction;
  }
  return toDirection(input);
}

std::string PlayerNumber::directionOptions() const {
  return
      "arrow keys\n"
      "w: Up\n"
      "a: Left\n"
      "s: Down\n"
      "d: Right\n";
}

std::string PlayerNumber::actionOptions() const {
  return
      "arrow keys: Move\n"
      "w: Move Up\n"
      "a: Move Left\n"
      "s: Move Down\n"
      "d: Move Right\n"
      "space: Fire an Arrow\n";
}



MultiplayerSetup::MultiplayerSetup(const Event* defaultEvent) :
    BasicSetup(defaultEvent) {}

void MultiplayerSetup::promptConfigurations() {
  BasicSetup::promptConfigurations();

  constexpr int maxPlayers = 5;
  numPlayers = BasicSetup::promptUserInputWithRange(
      "How many players?", 1, maxPlayers);
}

int MultiplayerSetup::getSetupVar(const int type) const {
  if(type == numPlayerSetupIndex) {
    return numPlayers;
  }
  return BasicSetup::getSetupVar(type);
}



GET_GAME_SETUP_FUNC {
  GameSetup* setup = new MultiplayerSetup(defaultEvent);
  return setup;
}

GET_PLAYERS_LIST_FUNC {
  constexpr char symbols[MultiplayerSetup::maxPlayers] = {
    '*', '~', '@', '^', '&'
  };

  int numPlayers = setup->getSetupVar(MultiplayerSetup::numPlayerSetupIndex);
  std::vector<Player*> players;
  for(int i = 0; i < numPlayers; ++i) {
    Player* pl = new PlayerNumber(i + 1, symbols[i]);
    players.push_back(pl);
  }

  return players;
}
