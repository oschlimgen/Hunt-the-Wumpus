#ifndef BABY_WUMPUS_HPP
#define BABY_WUMPUS_HPP


#include "event.hpp"

class BabyWumpus : public Event {
public:
  static constexpr const char* eventName = "BabyWumpus";

private:
  static constexpr double killChance = 0.5;

public:
  /*
   * Function: Constructor
   * Description: Creates this event with the given defaultEvent and room
   *    location.
   * Parameters:
   *    defaultEvent (Event*): Provides default behavior for the event.
   *    room (RoomPos): The location to place the event.
   */
  BabyWumpus(Event* defaultEvent, const RoomPos& room);

  /*
   * Function: clone
   * Description: Returns a new Event that is a copy of this event.
   * Returns (Event*): A pointer to a newly allocated Event object with the
   *    same derived class as this.
   * Effects: Allocates memory which must be deleted.
   */
  Event* clone() const override;

  /*
   * Function: character
   * Description: Returns the character that would be displayed in debug mode.
   * Returns (char): A character to display on the board.
   */
  char character() const override;

  /*
   * Function: name
   * Description: Returns the value of eventName.
   * Returns (string): The value of eventName.
   */
  std::string name() const override;

  /*
   * Function: getPercept
   * Description: Returns a string that warns the player of the event.
   * Returns (string): A warning to the player about the event.
   */
  std::string getPercept(int) const override;

  /*
   * Function: triggerUpdate
   * Description: Returns an update that either makes the player lose the game,
   *    or forces them to flee the room.
   * Parameters:
   *    trigger (Trigger*): The player that entered the room.
   * Returns (GameUpdate::pointer): Has a 50-50 chance to return an update
   *    that makes the player lose. Otherwise, returns an update that forces
   *    the player to use their next turn fleeing the room.
   */
  GameUpdate::pointer triggerUpdate(Trigger* trigger) override;
};

#endif
