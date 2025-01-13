#ifndef TEXT_TRIGGER_HPP
#define TEXT_TRIGGER_HPP

#include "trigger.hpp"


class InfoTrigger : public Trigger {
private:
  std::string text;

public:
  /*
   * Function: Text Constructor
   * Description: Initializes this trigger with the text to store, accessible
   *    through the name function.
   * Parameters:
   *    text (string): The text to store.
   */
  InfoTrigger(const std::string& text);
  /*
   * Function: Room Constructor
   * Description: Initializes this trigger with the room position to store,
   *    accessible through the getLocation function.
   * Parameters:
   *    text (string): The text to store.
   */
  InfoTrigger(const RoomPos& room);

  /*
   * Function: clone
   * Description: Returns a new Trigger that is a copy of this trigger.
   * Returns (Trigger*): A pointer to a newly allocated Event object with the
   *    same derived class as this.
   * Effects: Allocated memory that must be deleted.
   */
  Trigger* clone() const override;

  /*
   * Function: name
   * Description: Returns the stored text this trigger was initialized with.
   * Returns (string): The stored text.
   */
  std::string name() const override;
};

#endif