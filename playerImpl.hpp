#include "player.hpp"

class BasicPlayer : public Player {
public:
  /*
   * Function: Default Constructor
   * Description: Calls the default constructor of Player.
   */
  BasicPlayer();

  /*
   * Function: name
   * Description: Returns the name of the player. Will be displayed to
   *    distinguish between players in a multiplayer game.
   * Returns (string): The name of the player.
   */
  std::string name() const override;

  /*
   * Function: character
   * Description: Returns the character that is displayed on the board to
   *    represent the player.
   * Returns (char): A single character that represents the player.
   */
  char character(int mode) const override;
};
