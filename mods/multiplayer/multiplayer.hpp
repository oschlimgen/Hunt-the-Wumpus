
#include "gameSetup.hpp"
#include "player.hpp"


class PlayerOne : public Player {
public:
  /*
   * Function: Default Constructor
   * Description: Calls the default constructor of Player.
   */
  PlayerOne();

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

class PlayerTwo : public Player {
public:
  PlayerTwo();

  /*
   * Function: name
   * Description: Returns the name "Player 2," which is displayed to
   *    distinguish between players.
   * Returns (string): "Player 2"
   */
  std::string name() const override;

  /*
   * Function: character
   * Description: Returns the tilda character to represent Player 2.
   * Returns (char): '~'
   */
  char character(int mode) const override;

  /*
   * Functions: toDirection, toAction
   * Description: Maps the arrow keys to movement and directions, and the space
   *    bar to the fire action.
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


/*
 * Function: getPlayers
 * Description: The function called by the base game during setup to load the
 *    players into the game.
 * Parameters:
 *    setup (GameSetup): The GameSetup object containing parameters entered
 *      by the user.
 * Returns (vector<Player*>): A vector containing an instance of PlayerOne and
 *    an instance of PlayerTwo.
 * Effects: Allocates memory which must be deleted.
 */
extern "C" std::vector<Player*> getPlayers(const GameSetup* setup);

