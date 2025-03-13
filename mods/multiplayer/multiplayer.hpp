#include "loadMods.hpp"
#include "gameSetup.hpp"
#include "gameSetupImpl.hpp"
#include "player.hpp"


class PlayerNumber : public Player {
private:
  int playerNum;
  char representation;

public:
  /*
   * Function: Default Constructor
   * Description: Calls the default constructor of Player.
   */
  PlayerNumber(int playerNum, char representation);

  /*
   * Function: name
   * Description: Returns the name "Player 1," which is displayed to
   *    distinguish between players.
   * Returns (string): "Player 1"
   */
  std::string name() const override;

  /*
   * Function: character
   * Description: Returns the asterisk character to represent Player 1.
   * Returns (char): '*'
   */
  char character(int mode) const override;

  /*
   * Functions: toDirection, toAction
   * Description: Maps WASD to movement and directions, and the space bar to
   *    the fire action.
   * Parameters:
   *    input (int): The input key or key code.
   * Returns (int): A number corresponding to a direction or action as
   *    described by the Actions enum.
   */
  int toDirection(int input) const override;
  int toAction(int input) const override;

  /*
   * Functions: directionOptions, actionOptions
   * Description: Returns a list of valid keys the player can use to input a
   *    direction or take an action.
   * Returns (string): A list of keys the player can use and their effect.
   */
  std::string directionOptions() const override;
  std::string actionOptions() const override;
};


class MultiplayerSetup : public BasicSetup {
public:
  static constexpr int maxPlayers = 5;
  static constexpr int numPlayerSetupIndex = 4;

private:
  int numPlayers;

public:
  MultiplayerSetup(const Event* defaultEvent);

  void promptConfigurations() override;

  int getSetupVar(int type) const override;
};


/*
 * Function: getSetup
 * Description: The function called by the base game during setup to load the
 *    game setup object into the game.
 * Parameters:
 *    default (Event*): The event that should provide default functionality for
 *      all other events.
 * Returns (GameSetup*): A GameSetup object on which only the constructor has
 *    been called (no prompts have been displayed).
 * Effects: Allocates memory which must be deleted (by the base game).
 */
extern "C" GET_GAME_SETUP_FUNC;

/*
 * Function: getPlayers
 * Description: The function called by the base game during setup to load the
 *    players into the game.
 * Parameters:
 *    setup (GameSetup): The GameSetup object containing parameters entered
 *      by the user.
 * Returns (vector<Player*>): A vector containing an instance of PlayerOne and
 *    an instance of PlayerTwo.
 * Effects: Allocates memory which must be deleted (by the base game).
 */
extern "C" GET_PLAYERS_LIST_FUNC;
