#ifndef ARROW_TRIGGER_HPP
#define ARROW_TRIGGER_HPP

#include "trigger.hpp"


class ArrowTrigger : public Trigger {
public:
  static constexpr const char* triggerName = "Arrows";

public:
  /*
   * Function: Constructor
   * Description: Creates an arrow trigger at the given location.
   * Parameters:
   *    pos (RoomPos): The location to put it.
   */
  ArrowTrigger(const RoomPos& pos);
  /*
   * Function: Constructor
   * Description: Creates an arrow trigger at the given location with the given
   *    initial direction.
   * Parameters:
   *    pos (RoomPos): The location to put it.
   *    dir (int): The direction to fire it in.
   */
  ArrowTrigger(const RoomPos& pos, int dir);

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
   * Description: Returns the value of triggerName.
   * Returns (string): triggerName
   */
  std::string name() const override;
};

#endif