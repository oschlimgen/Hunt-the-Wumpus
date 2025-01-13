#ifndef ESCAPE_ROPE_HPP
#define ESCAPE_ROPE_HPP

#include "event.hpp"


class EscapeRope : public Event {
public:
  static constexpr const char* eventName = "EscapeRope";

public:
  /*
   * Function: Constructor
   * Description: Initializes this event with the given defaultEvent. The
   *    location must be set later.
   */
  EscapeRope(Event* defaultEvent);

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
   * Function: triggerUpdate
   * Description: Returns an update that checks if the player has the gold
   *    item. If they do, they win the game.
   * Parameters:
   *    trigger (Trigger*): The player that entered the room.
   * Returns (GameUpdate::pointer): A list of GameUpdates that checks if the
   *    player has the gold item, and has them win the game if they do.
   */
  GameUpdate::pointer triggerUpdate(Trigger* trigger) override;
};

#endif
