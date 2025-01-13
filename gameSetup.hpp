#ifndef GAME_SETUP_HPP
#define GAME_SETUP_HPP

#include "event.hpp"
#include "player.hpp"


class GameSetup {
public:
  enum SetupVariables {
    GameMode,
    Height,
    Width,
  };

protected:
  int gameMode;
  std::vector<int> dimensions;

  const Event* defaultEvent;
  std::vector<Event*> events;
  std::vector<Player*> players;

public:
  /*
   * Function: Constructor
   * Description: Initializes default event to the given value.
   */
  GameSetup(const Event* defaultEvent);

  /*
   * Function(s): Copy/Move Constructor/Assignment
   * Description: It should never be necessary to copy a GameSetup object as
   *    there should only ever be one instance of it.
   */
  GameSetup(const GameSetup&) = delete;
  GameSetup(GameSetup&&) = delete;
  GameSetup& operator=(const GameSetup&) = delete;
  GameSetup& operator=(GameSetup&&) = delete;

  /*
   * Function: Destructor
   * Description: Deletes any events and players left in the event and player
   *    arrays. After a successful initialization of Game, all pointers in the
   *    the events and players vectors should be set to null, and deleting them
   *    will have no effect.
   */
  virtual ~GameSetup();

  /*
   * Function: getSetupVar
   * Description: A flexible getter (so that it may be overriden by mods) for
   *    numbers entered by the user during a promptConfigurations() call. The
   *    default implementation returns the member corresponding to the enum
   *    SetupVariables.
   * Parameters:
   *    type (int): The variable to get. The default implementation gets a
   *      variable according to SetupVariables. If type doesn't correspond to
   *      any member variables, return NONE.
   * Returns (int): A value entered by the user during promptConfigurations().
   */
  virtual int getSetupVar(int type) const;

  /*
   * Function: getDefaultEvent
   * Description: Getter for the default event
   * Returns (Event*): The default event for providing functionality that is
   *    consistent across events.
   */
  Event* getDefaultEvent() const;
  
  /*
   * Function: getEvents
   * Description: Getter for the array of events.
   * Returns (vector<Event*>&): The array of events that should be added to the
   *    game. Each event in the array should have a position after init() has
   *    been called.
   */
  const std::vector<Event*>& getEvents() const;
  /*
   * Function: addEvent
   * Description: Takes a pointer to an Event and transfers it to the list of
   *    Events to create.
   * Parameters:
   *    ev (Event*&): A pointer to an object derived from Event to be placed on
   *      the board. The pointer is set to nullptr afterward (to prevent
   *      deletion).
   * Effects: One new instance of the event is added to the list of events to
   *    be created on setup.
   */
  void addEvent(Event*& ev);
  /*
   * Function: clearEvents
   * Description: Clears the array of events without freeing memory. Should
   *    only be called after all of the pointers in the events array have been
   *    transfered to an object that will manage the memory allocated.
   */
  void clearEvents();

  /*
   * Function: getPlayers
   * Description: Getter for the array of players.
   * Returns (vector<Player*>&): The array of players that should be added to
   *    the game. Each player in the array should have a position after init()
   *    has been called.
   */
  const std::vector<Player*>& getPlayers() const;
  /*
   * Function: addPlayer
   * Description: Takes a pointer to a Player and transfers it to the list of
   *    players.
   * Parameters:
   *    pl (Player*&): A pointer to an object derived from Player to be used
   *      while playing the game. The pointer is set to nullptr afterward (to
   *      prevent deletion).
   * Effects: One new instance of the player is added to the list of players to
   *    be used in the game.
   */
  void addPlayer(Player*& pl);
  /*
   * Function: clearPlayers
   * Description: Clears the array of players without freeing memory. Should
   *    only be called after all of the pointers in the events array have been
   *    transfered to an object that will manage the memory allocated.
   */
  void clearPlayers();

  /*
   * Function: promptConfigurations
   * Description: Prompts the user to enter certain numbers that will be used
   *    to set up the game. Default implementation prompt the user for a game
   *    width and height, and whether to play in debug mode.
   * Effects: Prints prompts to the console and waits for responses. Sets class
   *    member variables according to the responses.
   */
  virtual void promptConfigurations() = 0;

  /*
   * Function: init
   * Description: Places events and players at locations in the cave, and
   *    performs any other necessary setup tasks.
   * Effects: After calling the init function, this object should be ready to
   *    be passed to the Game constructor.
   */
  virtual void init() = 0;
};


#endif