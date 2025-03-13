#ifndef GAME_SETUP_IMPL_HPP
#define GAME_SETUP_IMPL_HPP

#include <type_traits>

#include "gameSetup.hpp"


class BasicSetup : public GameSetup {
protected:
  /*
   * Function: randomNumber
   * Description: Returns a random number between zero (inclusive) and the
   *    given maximum (exclusive).
   * Parameters:
   *    max (int): The exclusive maximum of the random number to be generated.
   * Returns (int): A random number between zero and the given maximum.
   */
  static int randomNumber(int max);

  /*
   * Function: chooseUnique
   * Description: Picks a given number of unique integers within a range from
   *    zero to a maximum number.
   * Parameters:
   *    maxNum (int): The maximum end of the range of numbers to pick,
   *      exclusive.
   *    toChoose (int): The number of random numbers to choose. Should be less
   *      than or equal to maxNum.
   * Returns (vector<int>): A sorted list of unique random numbers between zero
   *    and the given maximum.
   */
  static std::vector<int> chooseUnique(int maxNum, int toChoose);

  /*
   * Function: chooseUniqueUnordered
   * Description: Chooses a random rearrangement of the numbers 0 to toChoose.
   * Parameters:
   *    toChoose (int): The number of numbers to rearrange.
   * Returns (vector<int>): An arrangement of numbers.
   */
  static std::vector<int> chooseUniqueUnordered(int toChoose);

  /*
   * Function: promptUserInput, promptUserInputWithRange
   * Description: Prompts the user to input a number, then reprompts them until
   *    they ender an integer within the given range. promptUserInputWithRange
   *    appends the range to the prompt displayed.
   * Parameters:
   *    prompt (std::string): The prompt to display to the user, asking for a
   *      number.
   *    minValue (int): The minimum value the user can input.
   *    maxValue (int): The maximum value the user can input.
   * Returns (int): The number the user selected
   * Effects: Prints to the console some number of lines prompting the user to
   *    enter a number.
   */
  static int promptUserInput(const std::string& prompt, int minValue,
      int maxValue);
  static int promptUserInputWithRange(const std::string& prompt,
      int minValue, int maxValue);
  
  /*
   * Function: addEventType
   * Description: Adds one or more events of the given type to the array of
   *    events.
   * Parameters:
   *    <T> (typename): The type of event to construct and add.
   *    toAdd (int): The number of the event to add.
   * Effects: Adds the given type and number of events to the events array.
   */
  template<typename T, std::enable_if_t<
      std::is_base_of<Event, T>::value, bool> = true>
  void addEventType(int toAdd = 1) {
    // Don't add the event if it has the same name as another
    for(Event* ev : events) {
      if(ev->name() == T::eventName) {
        return;
      }
    }

    // The event doesn't have the same name as another, so add it
    for(int i = 0; i < toAdd; ++i) {
      Event* ev = new T(defaultEvent->clone());
      addEvent(ev);
    }
  }

public:
  /*
   * Function: Constructor
   * Description: Clones the passed default event and stores the pointer for
   *    later use in setup. Also seeds random.
   * Effects: Random is seeded with the system time.
   */
  BasicSetup(const Event* defaultEvent);

  /*
   * Function: promptConfigurations
   * Description: Prompts the user to enter certain numbers that will be used
   *    to set up the game. Default implementation prompt the user for a game
   *    width and height, and whether to play in debug mode.
   * Effects: Prints prompts to the console and waits for responses. Sets class
   *    member variables according to the responses.
   */
  void promptConfigurations() override;

  /*
   * Function: init
   * Description: Places events and players at locations in the cave, and
   *    performs any other necessary setup tasks.
   * Effects: After calling this function, this object should be ready to be
   *    passed to the Game constructor.
   */
  void init() override;
};

#endif
