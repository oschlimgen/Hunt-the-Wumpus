#ifndef EVENT_HPP
#define EVENT_HPP

#include "gameUpdatePointer.hpp"
#include "item.hpp"
#include "trigger.hpp"


class Event {
public:
  static constexpr const char* eventName = "";

protected:
  Event* defaultEvent;
  RoomPos room;

public:
  /*
   * Function: Constructor
   * Description: Links this object to the provided default event.
   */
  Event(Event* defaultEvent);
  /*
   * Function: Constructor
   * Description: Links this object to the provided default event, and sets the
   *    location to the given location.
   */
  Event(Event* defaultEvent, const RoomPos& room);

  /*
   * Function: Destructor
   * Description: Deletes the default event.
   */
  virtual ~Event();

  /*
   * Function: clone
   * Description: Returns a new Event that is a copy of this event.
   * Returns (Event*): A pointer to a newly allocated Event object with the
   *    same derived class as this.
   * Effects: Allocates memory which must be deleted.
   */
  virtual Event* clone() const = 0;

  /*
   * Function: setEventPointer
   * Description: Simple setter for defaultEvent.
   * Parameters:
   *    ev (Event*): The Event pointer to set defaultEvent to.
   */
  void setEventPointer(Event* ev);
  
  /*
   * Function: getLocation
   * Description: Simple getter for the room/position of the event.
   * Returns (RoomPos): The position of the event on the board.
   */
  const RoomPos& getLocation() const;
  /*
   * Function: setLocation
   * Description: Simple setter for the room/position of the event.
   * Parameters:
   *    loc (RoomPos): The location on the board to set the event's room to.
   */
  void setLocation(const RoomPos& loc);

  /*
   * Function: character
   * Description: Returns the character that would be displayed in debug mode.
   * Returns (char): A character to display on the board.
   */
  virtual char character() const = 0;
  /*
   * Function: character(int)
   * Description: Returns the character to display to the board, based on the
   *    game mode.
   * Parameters:
   *    mode (int): The game mode to display based off of
   * Returns (char): A character to display on the board.
   */
  virtual char character(int mode) const;

  /*
   * Function: name
   * Description: For standard events, returns the value of eventName of the
   *    derived class. May also be overloaded to return a stored string.
   * Returns (string): The value of eventName or a stored string.
   */
  virtual std::string name() const = 0;

  /*
   * Function: getPercept
   * Description: Returns a string that warns the player of the event. Will
   *    only be displayed if the player is close to the event.
   * Parameters:
   *    mode (int): The current game display mode.
   * Returns (string): A warning to the player about the event.
   */
  virtual std::string getPercept(int mode) const;

  /*
   * Function: getItem
   * Description: If the event is to give the player an item under any
   *    conditions, this function should return a new instance of the item
   *    to give them. Otherwise returns nullptr.
   * Returns (Item*): A newly allocated Item object that the event should
   *    give to the player under certain conditions.
   * Effects: Allocated memory that must be deleted.
   */
  virtual Item* getItem();

  /*
   * Function: turnUpdate
   * Description: A function that is called every turn for each event. This
   *    function may return a list of GameUpdates to execute before the player
   *    takes their action.
   * Parameters:
   *    player (Player*): The player whose turn it is.
   *    round (bool): Is true on the first turn of each round (where a turn is
   *      given to all players), and is false on each other player's turn in
   *      the round.
   * Returns (GameUpdate::pointer): A list of updates to execute before the
   *    player takes their action.
   */
  virtual GameUpdate::pointer turnUpdate(Player* player, bool round);

  /*
   * Function: triggerUpdate
   * Description: A function that is called every time a trigger or player
   *    moves into the room with this event. Returns a list of GameUpdates to
   *    perform.
   * Parameters:
   *    trigger (Trigger*): The trigger that entered the room.
   * Returns (GameUpdate::pointer): A list of GameUpdates to perform.
   */
  virtual GameUpdate::pointer triggerUpdate(Trigger* trigger) = 0;
};

#endif
