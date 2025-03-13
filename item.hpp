#ifndef ITEM_HPP
#define ITEM_HPP

#include "htwTypes.hpp"


class Item {
protected:
  int itemCount;

public:
  /*
   * Function: Constructor
   * Description: Sets itemCount to the given vaule
   */
  Item(int count);

  /*
   * Function: getCount
   * Description: Simple getter for itemCount.
   * Returns (int): The vaule of itemCount.
   */
  int getCount() const;

  /*
   * Function: updateCount
   * Description: Adds the given value to the itemCount.
   * Parameters:
   *    toAdd (int): The value to add to itemCount.
   */
  virtual void updateCount(int toAdd);
  
  /*
   * Function: isVisible
   * Description: Returns true if the item should be displayed to the player.
   * Parameters:
   *    mode (int): The current game display mode.
   * Returns: true if the item should be displayed to the player, and false
   *    otherwise.
   */
  virtual bool isVisible(int mode) const;

  /*
   * Function: name
   * Description: Returns the unique name of the item, to be used in
   *    comparisons to determine if item types are the same.
   * Returns (string): The name of the item.
   */
  virtual std::string name() const = 0;
};

#endif
