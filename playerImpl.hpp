#include "player.hpp"

class BasicPlayer : public Player {
protected:
  /*
   * Function: toDirection
   * Description: Returns a direction if the given character is a valid
   *    direction character (w/a/s/d) and NONE otherwise
   * Parameters:
   *     input (int): The character to check
   * Returns (int): Direction corresponding to the character or NONE.
   */
  int toDirection(int input) const;

  /*
   * Function: toAction
   * Description: Returns a number corresponding to Actions if the given
   *    character is a valid action input, and NONE otherwise.
   * Parameters:
   *    input (int): The character to check
   * Returns (int): Action associated with the character or NONE.
   */
  int toAction(int input) const;

  /*
   * Function: directionOptions
   * Description: Creates a string to be displayed to the user describing
   *    which keys they may press to move.
   * Returns (string): Describes the movement/direction keys to the user.
   */
  std::string directionOptions() const;

  /*
   * Function: actionOptions
   * Description: Creates a string to be displayed to the user describing
   *    which keys they may press to take an action on their turn.
   * Returns (string): Describes the action keys to the user.
   */
  std::string actionOptions() const;

  /*
   */
  GameUpdate::pointer getTurnAction(int input);

  /*
   */
  GameUpdate::pointer getDirectionAction(int input, Trigger* other);

  /*
   * Function: getFireAction
   * Description: Returns a list of GameUpdates that cause an arrow to be
   *    fired.
   * Returns (GameUpdate::pointer): A list of GameUpdates that cause an arrow
   *    to be fired.
   */
  GameUpdate::pointer getFireAction();


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
  
  /*
   * Function: handleInput
   * Description: Takes a character input in the form of an item with the
   *    given name, and returns an update based on the input name and
   *    character.
   * Paramters:
   *    info (Item*): An InfoItem containing the input type and the input
   *      character.
   *    other (Trigger*): May be nullptr. If it isn't, then the direction
   *      should be stored to the trigger.
   * Returns (GameUpdate::pointer): A list of GameUpdates to execute based on
   *    the player input.
   */
  GameUpdate::pointer handleInput(const Item* info, Trigger* other) override;
};
