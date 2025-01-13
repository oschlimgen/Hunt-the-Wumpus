#ifndef CAVE_HPP
#define CAVE_HPP

#include "room.hpp"
#include "player.hpp"
#include "gameSetup.hpp"


class Cave {
protected:
  int gameMode;

public:
  /*
   * Function: Default Constructor
   * Description: Initalizes gameMode to NONE.
   */
  Cave();

  /*
   * Function: Destructor
   * Description: Doesn't explicitly delete anything, but should be marked
   *    as virtual for derived classes.
   */
  virtual ~Cave() = default;

  /*
   * Function: setGameMode
   * Description: Simple setter for the gameMode variable.
   * Parameters:
   *    mode (int): The value to set gameMode to.
   */
  void setGameMode(int mode);

  /*
   * Function: turnDisplay
   * Description: Returns a string containing information to be displayed at
   *    the start of each turn.
   * Parameters:
   *    active (Player*): The player whose turn it is.
   * Returns (string): A string containing a textual representation of the
   *    active player's items, the game board, and any percepts.
   */
  virtual std::string turnDisplay(Player* active) = 0;

  /*
   * Function: getTurnUpdate
   * Description: Returns a list of updates triggered by the start of the turn,
   *    by calling the turnUpdate function for each event in the cave.
   * Parameters:
   *    active (Player*): The player whose turn it is.
   *    round (bool): True if it's the first turn of a new round, and false
   *      otherwise (to be used with multiplayer).
   * Returns (GameUpdate::pointer): A list of GameUpdate objects to be executed
   *    at the start of the turn.
   */
  virtual GameUpdate::pointer getTurnUpdate(Player* active, bool round) = 0;

  /*
   * Function: updateState
   * Description: Executes the given update by changing the board state
   *    according to the instructions it contains. Returns any updates
   *    triggered by these changes.
   * Parameters:
   *    update (GameUpdate): Contains information about how to change the
   *      board state.
   * Returns (GameUpdate::pointer): A list of updates triggered by the
   *    changes, to be executed next.
   * Effects: Changes the board state based on the given update.
   */
  virtual GameUpdate::pointer updateState(const GameUpdate& update) = 0;
};

#endif
