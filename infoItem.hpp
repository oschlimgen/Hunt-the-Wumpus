#ifndef INFO_ITEM_HPP
#define INFO_ITEM_HPP


#include "item.hpp"


class InfoItem : public Item {
private:
  std::string infoLabel;

public:
  /*
   * Function: Constructor
   * Description: Initializes this item with the desired name and integer
   *    data (stored in item count).
   * Parameters:
   *    name (string): The name of the item.
   *    data (int): The integer data to store in the item.
   */
  InfoItem(const std::string& name, int data);


  /*
   * Function: isVisible
   * Description: Overriden version always returns false because the player
   *    shouldn't see this info.
   * Parameters:
   *    mode (int): The current game display mode.
   * Returns: false
   */
  bool isVisible(int mode) const override;

  /*
   * Function: name
   * Description: Returns the name given to the item at construction.
   * Returns (string): The assigned name of the item.
   */
  std::string name() const override;
};

#endif
