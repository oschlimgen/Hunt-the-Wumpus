#ifndef WUMPUS_HPP
#define WUMPUS_HPP

#include "event.hpp"


class Wumpus : public Event {
public:
  static constexpr const char* eventName = "Wumpus";

public:
  /*
   * Function: Constructor
   * Description: Initializes this event with the given defaultEvent. The
   *    location must be set later.
   */
  Wumpus(Event* defaultEvent);

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
   * Description: If the trigger is a player, causes the player to lose. If
   *    the trigger is an arrow, the player wins if it hits the Wumpus, and
   *    the Wumpus moves to a new location if it misses.
   * Parameters:
   *    trigger (Trigger*): The player that entered the room, or the arrow
   *      fired.
   * Returns (GameUpdate::pointer): If the trigger is a player, a GameUpdate
   *    that causes the player to lose. If the trigger is an arrow, a
   *    GameUdpate that causes the player wins if it's in the same room as the
   *    Wumpus, and causes the Wumpus to move to a new location if it's in a
   *    different room.
   */
  GameUpdate::pointer triggerUpdate(Trigger* trigger) override;
};

#endif
