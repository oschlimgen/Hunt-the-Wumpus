#ifndef ARROW_EVENT_HPP
#define ARROW_EVENT_HPP

#include "event.hpp"


class ArrowEvent : public Event {
public:
  static constexpr const char* eventName = "Arrows";

public:
  /*
   * Function: Constructor
   * Description: Initializes this event with the given defaultEvent. The
   *    location must be set later.
   */
  ArrowEvent(Event* defaultEvent);

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
   * Function: getItem
   * Description: Returns a new ArrowItem object.
   * Returns (Item*): An ArrowItem.
   * Effects: Allocates memory.
   */
  Item* getItem() override;

  /*
   * Function: triggerUpdate
   * Description: Returns an update that adds an arrow to the player's items.
   * Parameters:
   *    trigger (Trigger*): The player that entered the room.
   * Returns (GameUpdate::pointer): A list of GameUpdates that add the arrow to
   *    the player's items and alerts them of their find.
   */
  GameUpdate::pointer triggerUpdate(Trigger* trigger) override;
};



#endif
