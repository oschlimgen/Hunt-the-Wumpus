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
   * Function: Implicit bool Conversion
   * Description: Evaluates to true if the itemCount is greater than zero, and
   *    evaluates to false otherwise.
   */
  operator bool();

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
  void updateCount(int toAdd);

  /*
   * Function: name
   * Description: Returns the unique name of the item, to be used in
   *    comparisons to determine if item types are the same.
   * Returns (string): The name of the item.
   */
  virtual std::string name() const = 0;
};

#endif
