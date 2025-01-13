#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "cave.hpp"


// Game interface
class Game {
private:
  Cave* cave;

  int playing;
  std::vector<Player*> players;

  bool roundStart;
  // Saves any updates that weren't finished the previous turn
  GameUpdate::pointer multiTurnUpdates;

  bool boardDisplayed;
  std::string toDisplay;

  /*
   * Function: playerCount
   * Description: Returns the number of players that haven't lost the game.
   * Returns (int): The number of players that haven't lost.
   */
  int playerCount() const;

  /*
   * Function: activePlayer
   * Description: Returns a pointer to the active player based on the playing
   *    index variable.
   * Returns (Player*): A pointer to the active player.
   */
  Player* activePlayer();

  /*
   * Function: getNextPlayer
   * Description: Returns the next alive player in the players list, wrapping
   *    if necessary.
   * Returns (int): The player whose turn it would be next.
   */
  int getNextPlayer(int pl);

  /*
   * Function: swapTurn
   * Description: Switches the turn marker playing to the next player, wrapping
   *    if necessary.
   * Effects: A new playing player is assigned, the active state of the players
   *    is updated, roundStart may be set to true.
   */
  void swapTurn();

  /*
   * Function: displayGame
   * Description: Displays the game grid in the terminal.
   */
  void displayGame();

  /*
   * Function: checkWin
   * Description: Determines whether the player has won the game
   * Returns (bool): True if the player has won, false otherwise
   */
  bool checkWin() const;

  /*
   * Function: checkLose
   * Description: Determines whether the player has lost the game
   * Returns (bool): True if the player has lost, false otherwise
   */
  bool checkLose() const;

  /*
   * Function: confirmExit
   * Description: Prompts the user to make sure they actually want to exit
   *    the game.
   * Returns (bool): True if the player wishes to exit, and false otherwise.
   * Effects: Displays a prompt and waits for a response.
   */
  static bool confirmExit();

  /*
   * Function: turnPrompt
   * Description: Returns an update sequence that displays a prompt to take a
   *    turn action, then waits for an action input.
   * Returns (GameUpdate::pointer): The updates to perform that will prompt the
   *    user for an action.
   */
  GameUpdate::pointer turnPrompt();

  /*
   * Function: passUpdate
   * Description: Moves forward in the update linked list until the
   *    End event is encountered, or the end of the list.
   * Parameters:
   *    list (GameUpdate::pointer): A pointer to the linked list
   *    updateType (int): The event to end on
   * Returns (bool): True if it reached the end of the list, and false
   *    otherwise.
   * Effects: Increments list past the next End event
   */
  bool passUpdate(GameUpdate::pointer& list, int updateType);

  /*
   * Function: getUpdatesUntilType
   * Description: Moves forward in the update linked list until the End event
   *    is encountered, or the end of the list. Returns a list of each event
   *    passed this way.
   * Parameters:
   *    list (GameUpdate::pointer): A pointer to the linked list
   *    updateType (int): The event to end on
   * Returns (GameUpdate::pointer): Each event passed while searching for the
   *    End event.
   * Effects: Increments list past the next End event.
   */
  GameUpdate::pointer getUpdatesUntilType(GameUpdate::pointer& list,
      int updateType);
  
  /*
   * Function: getWaitUpdates
   * Description: Moves forward in the update linked list until the EndWait
   *    event is encountered, or the end of the list. Returns a list of each
   *    event passed this way, and any other events produced from the
   *    WaitForMove event.
   * Parameters:
   *    update (GameUpdate::pointer): A pointer to the linked list
   * Returns (GameUpdate::pointer): Each event passed while searching for the
   *    EndWait event and any events added by the info in the WaitForMove.
   * Effects: Increments list past the next EndWait event.
   */
  GameUpdate::pointer getWaitUpdates(GameUpdate::pointer& update);

  /*
   * Function: insertMultiTurn
   * Description: Inserts a list of updates the appropriate number of turn
   *    waits into the multiTurnUpdate list.
   * Parameters:
   *    update (GameUpdate::pointer): The list of updates to insert.
   *    waitCount (int): The number of turns to insert the wait after.
   * Effects: Adds the given list of updates to multiTurnUpdates.
   */
  void insertMultiTurn(const GameUpdate::pointer& update, int waitCount);


  /*
   * Function: startTurnUpdate
   * Description: Returns the GameUpdates to start the turn with. Usually this
   *    will be the prompt GameUpdates returned by turnPrompt().
   * Returns: The GameUpdates to execute 
   */
  GameUpdate::pointer startTurnUpdate();

  /*
   * Function: resolveUpdate
   * Description: Executes the update action or delegates its resolution to
   *    the Cave object. Gets any further updates to perform triggered by the
   *    GameUpdate's resolution.
   * Parameters:
   *    update (GameUpdate): The update to resolve.
   * Returns (GameUpdate::pointer): Either a list of updates to add to the
   *    queue or evaluates to false.
   */
  GameUpdate::pointer resolveUpdate(const GameUpdate& update);

  /*
   * Function: resolveTurn
   * Description: Executes a full turn of the game.
   * Returns: A boolean that is true if the game should continue and false if
   *    it should terminate.
   */
  bool resolveTurn();

public:
  /*
   * Function: Constructor
   * Description: Constructs Game object with parameters given by the GameSetup
   *    object.
   * Parameters:
   *    setup (GameSetup*): An object derived from GameSetup that provides
   *      parameters used to set up the game object.
   *    cave (Cave*): The cave object that stores the game board.
   */
  Game(GameSetup* setup, Cave* cave);

  /*
   * Functions: Copy/Move Constructor/Assignment
   * Description: The Game object shouldn't need to copied ever. These 
   *    functions are deleted beacuse this object contains pointers.
   */
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;

  /*
   * Function: Destructor
   * Description: Frees memory allocated to players and multi-turn updates.
   *    Closes the curses library if in use.
   */
  ~Game();

  /*
   * Function: playGame
   * Description: Runs one full iteration of the game.
   */
  void playGame();
};
#endif
