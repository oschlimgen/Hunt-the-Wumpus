#ifndef GOLD_ITEM_HPP
#define GOLD_ITEM_HPP

#include "item.hpp"


class GoldItem : public Item {
public:
  static constexpr const char* itemName = "Gold";

public:
  
  /*
   * Function: Default Constructor
   * Description: Creates an Item with itemCount 1.
   */
  GoldItem();
  /*
   * Function: Constructor
   * Description: Creates an Item with the given itemCount.
   * Parameters:
   *    count (int): The number of gold items to give to the player.
   */
  GoldItem(int count);

  /*
   * Function: name
   * Description: Returns the value of itemName.
   * Returns (string): itemName
   */
  std::string name() const override;
};

#endif
