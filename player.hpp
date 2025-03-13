#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "trigger.hpp"
#include "item.hpp"
#include "gameUpdate.hpp"
#include "gameUpdatePointer.hpp"


class Player : public Trigger {
public:
  enum State {
    LostGame = 0,
    WonGame = 1,
    Active = 2
  };

  static constexpr const char* turnActionID = "TurnAction";

protected:
  // 0: Lost, 1: Won, 2: Active, NONE: Still Playing
  int state;
  std::vector<Item*> items;

public:
  /*
   * Function: Default Constructor
   * Description: Sets the room of the player to NONE, room should be set
   *    at some later point.
   */
  Player();
  /*
   * Function: Constructor
   * Description: Sets the room of the player to the given room.
   */
  Player(const RoomPos& room);

  /*
   * Function: Destructor
   * Description: Deletes all items the player has.
   */
  virtual ~Player();

  /*
   * Function: clone
   * Description: Returns nullptr because Player objects should never be
   *    cloned.
   * Returns (Trigger*): nullptr
   */
  Trigger* clone() const override;

  /*
   * Function: player
   * Description: Because this object is a player, return this object as
   *    a valid player pointer.
   */
  Player* player() override;
  
  /*
   * Function: getState
   * Description: Getter for the state of the player.
   * Returns (int): The state of the player.
   */
  int getState() const;
  /*
   * Function: setState
   * Description: Setter for the state of the player.
   * Parameters:
   *    curState (int): The new state to set state to.
   */
  void setState(int curState);
  
  /*
   * Function: addItem
   * Description: If an item with the given item's name already exists, adds
   *    their quantities and deletes the item. If it doesn't exist in the items
   *    list, adds it to the list.
   * Parameters:
   *    item (Item*): The item to add to the player. Shouldn't be de-referenced
   *      after the function call.
   * Effects: Adds to the item vector, and sets passed item pointer to nullptr.
   */
  void addItem(Item* item);

  /*
   * Function: getItem
   * Description: Returns a pointer to the item in the player's item list if it
   *    exists.
   * Parameters:
   *    name (string): The name of the item to search for.
   * Returns (Item*): A pointer to the item with the given name, or returns
   *    nullptr if it isn't found in the player's item list.
   */
  Item* getItem(const std::string& name);
  
  /*
   * Function: removeItem
   * Description: Removes the item with the given name from the player's item
   *    list.
   * Parameters:
   *    name (string): The name of the item to remove.
   * Returns (bool): True if an item with the given name is found.
   * Effects: Removes any items with the given name from the list of items.
   */
  bool removeItem(const std::string& name);

  /*
   * Function: character
   * Description: Gets the character associated with the player to mark their
   *    position.
   * Parameters:
   *    mode (int): The game display mode.
   * Returns (char): A character representing the player.
   */
  virtual char character(int mode) const = 0;

  /*
   * Function: getItemList
   * Description: Returns a text list of items the player has.
   * Parameters:
   *    mode (int): The game display mode.
   * Returns (string): A list of items and the associated count,
   *    separated by new lines.
   */
  virtual std::string getItemList(int mode) const;

  /*
   * Function: turnUpdate
   * Description: Is called at the beginning of each turn. If any events need
   *    to happen on a turn-by-turn basis, they should be returned by this
   *    function.
   * Parameters:
   *    active (Player*): The player whose turn it is.
   *    round (bool): True if a new round is starting, and false otherwise.
   * Returns (GameUpdate::pointer): Any updates that should be executed on a
   *    turn-by-turn basis. Default is nullptr.
   */
  virtual GameUpdate::pointer turnUpdate(Player* active, bool round);

  /*
   * Function: triggerUpdate
   * Description: Will be called when a trigger moves to the same room as the
   *    player, allows for interaction.
   * Parameters:
   *    trigger (Trigger*): The trigger to react to.
   * Returns (GameUpdate::pointer): Any modifications that should be made to
   *    the game state.
   */
  virtual GameUpdate::pointer triggerUpdate(Trigger* trigger);
};

#endif
