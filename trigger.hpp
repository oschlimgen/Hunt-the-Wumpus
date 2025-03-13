#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include "gameUpdate.hpp"
#include "item.hpp"


class Player;

class Trigger {
public:
  static constexpr const char* triggerName = nullptr;
  static constexpr const char* promptTriggerDirectionID = "TriggerDirection";

protected:
  RoomPos room;
  int moveDir;

  bool enabled;

public:
  /*
   * Function: Constructor
   * Description: Sets the position of the Trigger to the given room, and
   *    enabled to true.
   */
  Trigger(const RoomPos& room);

  /*
   * Function: Destructor
   * Description: Doesn't explicitly delete anything, but must be declared as
   *    virtual.
   */
  virtual ~Trigger();

  /*
   * Function: clone
   * Description: Returns a new Trigger that is a copy of this trigger.
   * Returns (Trigger*): A pointer to a newly allocated Event object with the
   *    same derived class as this.
   * Effects: Allocated memory that must be deleted.
   */
  virtual Trigger* clone() const = 0;

  /*
   * Function: player
   * Description: Returns a valid pointer if this Trigger object is a Player.
   * Returns (Player*): A valid pointer if this Trigger object is a Player, and
   *    nullptr otherwise.
   */
  virtual Player* player();

  /*
   * Function: name
   * Description: For standard triggers, returns the value of triggerName of
   *    the derived class. May also be overloaded to return a stored string.
   * Returns (string): The value of triggerName or a stored string.
   */
  virtual std::string name() const = 0;

  /*
   * Function: getLocation
   * Description: Simple getter for the room the trigger is in.
   * Returns (RoomPos): The position of the trigger.
   */
  const RoomPos& getLocation() const;
  /*
   * Function: setLocation
   * Description: Simple setter for the room the trigger is in.
   * Parameters:
   *    loc (RoomPos): The room to set the position of the trigger to.
   */
  void setLocation(const RoomPos& loc);

  /*
   * Function: getDirection
   * Description: Simple getter for moveDir.
   * Returns (int): The value of moveDir.
   */
  int getDirection() const;
  /*
   * Function: setDirection
   * Description: Simple setter for moveDir.
   * Parameters:
   *    dir (int): The value to set moveDir to.
   */
  void setDirection(int dir);

  /*
   * Function: getEnabled
   * Description: Simple getter for enabled.
   * Returns (bool): The value of enabled.
   */
  bool getEnabled() const;
  /*
   * Function: setEnabled
   * Description: Simple setter for enabled.
   * Parameters:
   *    state (bool): The value to set enabled to.
   */
  void setEnabled(bool state);
  
  /*
   * Function: handleInput
   * Description: Takes a character input in the form of an item with the
   *    given name, and returns an update based on the input name and
   *    character.
   * Paramters:
   *    info (Item*): An InfoItem containing the input type and the input
   *      character.
   *    other (Trigger*): The player to get input keys from, or the trigger
   *      to modify.
   * Returns (GameUpdate::pointer): A list of GameUpdates to execute based on
   *    the player input.
   */
  virtual GameUpdate::pointer handleInput(const Item* info, Trigger* other);
};

#endif
