#ifndef ARROW_ITEM_HPP
#define ARROW_ITEM_HPP

#include "item.hpp"


class ArrowItem : public Item {
public:
  static constexpr const char* itemName = "Arrows";

public:
  /*
   * Function: Default Constructor
   * Description: Creates an Item with itemCount 1.
   */
  ArrowItem();
  /*
   * Function: Constructor
   * Description: Creates an Item with the given itemCount.
   * Parameters:
   *    count (int): The number of arrows to give to the player.
   */
  ArrowItem(int count);

  /*
   * Function: name
   * Description: Returns the value of itemName.
   * Returns (string): itemName
   */
  std::string name() const override;
};

#endif