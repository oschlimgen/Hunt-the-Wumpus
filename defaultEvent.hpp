#ifndef EVENT_IMPL_HPP
#define EVENT_IMPL_HPP

#include "event.hpp"


class DefaultEvent : public Event {
public:
  static constexpr const char* eventName = "DefaultEvent";

public:
  /*
   * Function: Constructor
   * Description: Calls the event constructor with nullptr for defaultEvent.
   */
  DefaultEvent();

  /*
   * Function: Destructor
   * Description: Must set defaultEvent member to nullptr to prevent deletion
   *    of the event reference, which could lead to recursive destructor calls.
   */
  ~DefaultEvent() override;

  /*
   * Function: clone
   * Description: Returns a default-constructed DefaultEvent object.
   * Returns (Event*): A newly allocated DefaultEvent object.
   * Effects: Allocates memory which must be deleted.
   */
  Event* clone() const override;

  /*
   * Function: character
   * Description: Returns NONE.
   * Returns (char): NONE
   */
  char character() const override;

  /*
   * Function: character
   * Description: Returns the character of the linked event or NONE based on
   *    the game display mode.
   * Parameters:
   *    mode (int): The current display mode of the game.
   * Returns (char): Character of the linked event if in debug mode, and NONE
   *    otherwise.
   */
  char character(int mode) const override;

  /*
   * Function: name
   * Description: Returns the value of eventName.
   * Returns (string): The value of eventName.
   */
  std::string name() const override;

  /*
   * Function: triggerUpdate
   * Description: Returns nullptr because there is no default GameUpdate to
   *    perform when a trigger moves.
   * Returns (GameUpdate::pointer): nullptr
   */
  GameUpdate::pointer triggerUpdate(Trigger* trigger) override;
};

#endif