#ifndef ROOM_HPP
#define ROOM_HPP

#include "event.hpp"
#include "player.hpp"

// Room interface
class Room {
private: 
  RoomPos pos;
  std::vector<Event*> events;
  std::vector<Player*> players;

  /*
   * Function: randIndex
   * Description: Generates a random number index of a vector with the given
   *    size.
   * Parameters:
   *    size (int): The maximum value of the random number, exclusive.
   * Returns (int): A random number between 0 and size, not including size
   *    itself.
   */
  static int randIndex(int size);

public:
  /*
   * Function: Default Constructor
   * Description: Calls default constructors of all member variables.
   */
  Room();
  /*
   * Function: Constructor
   * Description: Sets the room position to the given position.
   */
  Room(const RoomPos& pos);

  /*
   * Function: Copy Constructor
   * Description: Copies all values and clones all events from the other room.
   * Parameters:
   *    other (Room): The Room object to copy values from.
   */
  Room(const Room& other);
  /*
   * Function: Copy Assignment
   * Description: Copies all values and clones all events from the other room.
   * Parameters:
   *    other (Room): The Room object to copy values from.
   */
  Room& operator=(const Room& other);

  /*
   * Function: Destructor
   * Description: Deletes all events assigned to the room.
   */
  ~Room();

  /*
   * Function: getLocation
   * Description: Simple getter for the position of the room.
   * Returns (RoomPos): The position of the room.
   */
  const RoomPos& getLocation() const;
  /*
   * Function: setLocation
   * Description: Simple setter for the position of the room.
   * Parameters:
   *    loc (RoomPos): The position to assign to the room.
   */
  void setLocation(const RoomPos& loc);

  /*
   * Function: empty
   * Description: Returns true if the room has no events other than the default
   *    event.
   * Returns (bool): True if the room only has one event, and false otherwise.
   */
  bool empty() const;

  /*
   * Function: getEvent
   * Description: Returns a pointer to the first event with the given name in
   *    the list of events in the room.
   * Parameters:
   *    eventName (string): The name to compare against each event's name.
   * Returns (Event*): An event with a matching name or nullptr if not found.
   */
  Event* getEvent(const std::string& eventName);

  /*
   * Function: addEvent
   * Description: Appends the given event to the list of events.
   * Parameters:
   *    event (Event*): The event to add. The pointer remains valid and points
   *      to the added event.
   */
  void addEvent(Event* event);
  /*
   * Function: removeEvent
   * Description: Searches the event list for the exact Event pointer, then
   *    removes it if found. Does NOT delete the pointer.
   * Parameters:
   *    event (Event*): The event to search for a matching pointer for.
   */
  void removeEvent(Event* event);

  /*
   * Function: addPlayer
   * Description: Appends the given player to the list of players.
   * Parameters:
   *    player (Player*): The player to add. The pointer remains valid and points
   *      to the added player.
   */
  void addPlayer(Player* player);
  /*
   * Function: removePlayer
   * Description: Searches the players list for the exact Player pointer, then
   *    removes it if found. Does NOT delete the pointer.
   * Parameters:
   *    player (Player*): The player to search for a matching pointer for.
   */
  void removePlayer(Player* player);

  /*
   * Function: getChar
   * Description: Searches all the players and events in the the room for a
   *    character to display. If all has a character of NONE, NONE is returned.
   * Parameters:
   *    mode (int): The game display mode.
   * Returns (char): The character representing the room, based on the players
   *    and events it contains. Preference is given to players for character
   *    display.
   */
  char getChar(int mode) const;

  /*
   * Function: getPercepts
   * Description: Returns a string containing each percept from events in the
   *    room in a random order.
   * Parameters:
   *    mode (int): The game display mode.
   * Returns (string): The string to display, containing each percept from the
   *    room. Randomly orders percepts if there are multiple.
   */
  std::string getPercepts(int mode) const;

  /*
   * Function: triggerTurnUpdate
   * Description: Calls the turnUpdate function for each player and event in
   *    the room.
   * Parameters:
   *    active (Player*): The player whose turn it is.
   *    round (bool): True if it is the first turn of a round.
   * Returns (GameUpdate::pointer): The list of GameUpdates triggered by the
   *    change of turn.
   */
  GameUpdate::pointer triggerTurnUpdate(Player* active, bool round);

  /*
   * Function: triggerEvents
   * Description: Calls the triggerUpdate function for each player and event in
   *    the room.
   * Parameters:
   *    trigger (Trigger*): The trigger that entered the room or otherwise
   *      triggered its events.
   * Returns (GameUpdate::pointer): The list of GameUpdates to perform due to
   *    the trigger.
   */
  GameUpdate::pointer triggerEvents(Trigger* trigger);
};

#endif
