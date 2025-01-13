#ifndef TEXT_EVENT_HPP
#define TEXT_EVENT_HPP

#include "event.hpp"


class InfoEvent : public Event {
private:
  std::string text;

public:
  /*
   * Function: Text Constructor
   * Description: Initializes this event with the text to store, accessible
   *    through the name function.
   * Parameters:
   *    text (string): The text to store.
   */
  InfoEvent(const std::string& text);
  /*
   * Function: Room Constructor
   * Description: Initializes this event with the room position to store,
   *    accessible through the getLocation function.
   * Parameters:
   *    text (string): The text to store.
   */
  InfoEvent(const RoomPos& room);

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
   * Description: Character functions should always return NONE, since the text
   *    has no effect on the board.
   * Returns (char): Always NONE.
   */
  char character() const override;
  char character(int) const override;

  /*
   * Function: name
   * Description: Returns the stored text this event was initialized with.
   * Returns (string): The stored text.
   */
  std::string name() const override;

  /*
   * Function: triggerUpdate
   * Description: Should always return nullptr, since the text has no effect
   *    on the board.
   * Returns (GameUpdate::pointer): Always nullptr.
   */
  GameUpdate::pointer triggerUpdate(Trigger* trigger) override;
};

#endif
