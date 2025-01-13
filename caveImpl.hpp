#ifndef CAVE_BASIC_IMPL_HPP
#define CAVE_BASIC_IMPL_HPP

#include "cave.hpp"


class BasicCave : public Cave {
private:
  static constexpr int arrowFireDist = 3;

private:
  int height;
  int width;
  std::vector<std::vector<Room>> cave;
  
  // Determines whether the playing player will be prompted for an action
  bool playerActionEnabled;


  /*
   * Function: getRoom
   * Description: Returns the room in the cave at RoomPos.
   * Parameters:
   *    pos (RoomPos): The position to get the room at.
   * Returns (Room&): A reference to the room at the given position.
   */
  Room& getRoom(const RoomPos& pos);
  const Room& getRoom(const RoomPos& pos) const;
  
  /*
   * Function: getAction
   * Description: Waits for an action from the user in the form of a key press.
   * Returns (int): The (possibly invalid) action provided by the player.
   */
  int getAction();
  
  /*
   * Function: addDirection
   * Description: Returns the room adjacent to the given room in the given
   *    direction.
   * Parameters:
   *    room (RoomPos): The original room.
   *    direction (int): The direction corresponding to the Direction enum
   *      to move in.
   * Returns (RoomPos): The adjacent room.
   */
  RoomPos addDirection(const RoomPos& room, int direction) const;

  /*
   * Function: chooseRoom
   * Description: Randomly chooses a room in the cave (not necessarily empty)
   *    and returns its position.
   * Returns (RoomPos): The position of the randomly chosen room.
   */
  RoomPos chooseRoom();

  /*
   * Function: chooseEmptyRoom
   * Description: Randomly chooses an empty room in the cave and returns its
   *    position.
   * Returns (RoomPos): The position of the randomly chosen room.
   */
  RoomPos chooseEmptyRoom();
  
  /*
   * Function: chooseDirection
   * Description: Randomly chooses a direction that the object at the given
   *    location could move in.
   * Parameters:
   *    pos (RoomPos): The location of the object.
   * Returns (int): A number corresponding to the randomly chosen direction.
   */
  int chooseDirection(const RoomPos& pos);

  /*
   * Function: chooseEmptyDirection
   * Description: Randomly chooses a direction that the object at the given
   *    location could move in, and the adjacent room in that direction is
   *    empty.
   * Parameters:
   *    pos (RoomPos): The location of the object.
   * Returns (int): A number corresponding to the randomly chosen direction.
   */
  int chooseEmptyDirection(const RoomPos& pos);


  /* 
   * Function: getItems
   * Description: Returns a string of items to display.
   * Parameters:
   *    player (Player*): The player to get the items of.
   * Returns (string): A textual representation of the player's items.
   */
  std::string getItems(Player* player) const;

  /*
   * Function: getBoard
   * Description: Returns a string that will be printed to the console that
   *    displays the board.
   * Parameters:
   *    players (vector<Player*>): The players in the game to display on
   *      the board.
   * Returns (string): A textual representing the board.
   */
  std::string getBoard() const;

  /*
   * Function: getPercepts
   * Description: Returns a string that will be printed to the console that
   *    contains any percepts that the player should recieve.
   * Parameters:
   *    player (Player*): The player to get percepts for.
   * Returns (string): A textual representation of the player's percepts.
   */
  std::string getPercepts(Player* player) const;
  
  /*
   * Function: getActionUpdate
   * Description: Creates one or more GameUpdate objects that describe how the
   *    game state should be changed based on the player's input.
   * Parameters:
   *    player (Player*): The active player
   *    actionInput (int): The key from getch() entered by the player
   * Returns (GameUpdate::pointer): The updates to perform to the game state.
   */
  GameUpdate::pointer actionUpdate(Player* player, int actionInput);
  
  /*
   * Function: fireArrow
   * Description: Prompts the user for a directino, then fires an arrow in the
   *    direction specified
   * Parameters:
   * Post-condition: Arrow is fired. Wumpus is killed if hit and moves if
   *     missed.
   */
  GameUpdate::pointer fireArrow(Player* active);
  
  /*
   * Function: promptForPlayerAction
   * Description: Reprompts the user through the update loop until they enter
   *    a valid input action, then returns GameUpdate objects corresponding to
   *    the given action.
   * Parameters:
   *    player (Player*): The player to use direction conversion for.
   * Returns (GameUpdate::pointer): GameUpdates describing how to resolve the
   *    chosen action. If an invalid action was entered, returns GameUpdates
   *    describing how to re-prompt the user.
   */
  GameUpdate::pointer promptForPlayerAction(const GameUpdate& update);
  
  /*
   * Function: handleDirectionInput
   * Description: If the player entered a direction character, sets the
   *    direction of the provided trigger or player to the direction entered.
   * Parameters:
   *    update (GameUpdate): The PromptDirection update containing the trigger
   *      or player to apply the direction to.
   *    action (int): The code of the character entered.
   * Returns (GameUpdate::pointer): The update to execute based on what the
   *    player entered.
   */
  GameUpdate::pointer handleDirectionInput(const GameUpdate& update,
      int action);

  /*
   * Function: promptForDirection
   * Description: Reprompts the user through the update loop until they enter
   *    a valid direction, then assigns that value to the update trigger.
   * Parameters:
   *    player (Player*): The player to use direction conversion for.
   *    update (GameUpdate): The update containing the trigger to set the
   *      direction of.
   * Returns (GameUpdate::pointer): GameUpdates describing how to re-prompt the
   *    player if necessary. If a valid direction was entered, returns nullptr.
   */
  GameUpdate::pointer promptForDirection(const GameUpdate& update);

  /*
   * Function: createObject
   * Description: Adds the object contained by the GameUpdate object to the
   *    cave board. (The object should be an event.)
   * Parameters:
   *    update (GameUpdate): The update containing the object to add.
   * Effects: Adds the object to the cave.
   */
  void createObject(const GameUpdate& update);

  /*
   * Function: moveRandom
   * Description: Chooses a random room based on the restrictions associated
   *    with the given info code.
   * Parameters:
   *    from (RoomPos): The current location of the object to move.
   *    info (int): The code corresponding to the restrictions to place on the
   *      random room: 0 - Any, 1 - Empty, 2 - Adjacent, 3 - Empty Adjacent.
   * Returns (RoomPos): The position of the chosen room.
   */
  RoomPos moveRandom(const RoomPos& from, int info);

  /*
   * Function: movePlayer
   * Description: Moves the player to a new room in the cave, using the
   *    information provided in the GameUpdate object. Triggers any events
   *    in the room they're moved to.
   * Parameters:
   *    update (GameUpdate): Contains information about how to move the player.
   * Returns (GameUpdate::pointer): A list of updates to execute, triggered by
   *    moving the player.
   * Effects: The player is removed from the room that they are in, and placed
   *    in a new one. The player's position member is updated to reflect this
   *    change.
   */
  GameUpdate::pointer movePlayer(const GameUpdate& update);
  /*
   * Function: moveEvent
   * Description: Moves the event to a new room in the cave, using the
   *    information provided in the GameUpdate object.
   * Parameters:
   *    update (GameUpdate): Contains information about how to move the event.
   * Returns (GameUpdate::pointer): Should return nullptr because events can't
   *    trigger other events. Returns a value for uniformity.
   * Effects: The event is removed from the room that it's in, and placed in a
   *    new one. The event's position member is updated to reflect this change.
   */
  GameUpdate::pointer moveEvent(const GameUpdate& update);
  /*
   * Function: moveTrigger
   * Description: Moves the trigger to a new room in the cave, using the
   *    information provided in the GameUpdate object. Triggers any events
   *    in the room it's moved to.
   * Parameters:
   *    update (GameUpdate): Contains information about how to move the trigger.
   * Returns (GameUpdate::pointer): A list of updates to execute, triggered by
   *    moving the trigger.
   * Effects: The trigger's position member is updated.
   */
  GameUpdate::pointer moveTrigger(const GameUpdate& update);

  /*
   * Function: moveObject
   * Description: Selects a move function to call based on what object the
   *    update contains, then returns its result.
   * Parameters:
   *    update (GameUpdate): The MoveObject update containing the object to
   *      move.
   * Returns (GameUpdate::pointer): The update triggered by the object moving,
   *    if there is one.
   */
  GameUpdate::pointer moveObject(const GameUpdate& update);

  /*
   * Function: destroyObject
   * Description: Removes the object contained by the GameUpdate object from
   *    the cave board. (The object should be an event.)
   * Parameters:
   *    update (GameUpdate): The update containing the object to remove.
   * Effects: Removes the object from the cave.
   */
  void destroyObject(const GameUpdate& update);

  /*
   * Function: evaluateItemConditional
   * Description: Checks if the given player has the required item. If they
   *    don't, it will return a BeginConditional update.
   * Parameters:
   *    update (GameUpdate): Contains the player to check, item name to check
   *      for, and possibly number to check for.
   * Returns (GameUpdate::pointer): BeginConditional update if it failed the
   *    check, and nullptr otherwise.
   */
  GameUpdate::pointer evaluateItemConditional(const GameUpdate& update);

  /*
   * Function: triggerEventWithEvent
   * Description: Searches the cave for events with the given week, and
   *    triggers each it finds with the given trigger.
   * Parameters:
   *    update (GameUpdate): Contains the name of the event to search for and
   *      the trigger to trigger the event with.
   * Returns (GameUpdate::pointer): The result of triggering the event.
   */
  GameUpdate::pointer triggerEventWithName(const GameUpdate& update);

public:
  /*
   * Function: Constructor
   * Description: Creates the cave based on setup variables from the user and
   *    events that should be placed in the cave.
   * Parameters:
   *    setup (GameSetup*): Contains setup variables and events to use to
   *      create the game board.
   */
  BasicCave(const GameSetup* setup);

  /*
   * Function: turnDisplay
   * Description: Returns a string containing information to be displayed at
   *    the start of each turn.
   * Parameters:
   *    active (Player*): The player whose turn it is.
   * Returns (string): A string containing a textual representation of the
   *    active player's items, the game board, and any percepts.
   */
  std::string turnDisplay(Player* active) override;

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
  GameUpdate::pointer getTurnUpdate(Player* active, bool round) override;

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
  GameUpdate::pointer updateState(const GameUpdate& update) override;
};

#endif