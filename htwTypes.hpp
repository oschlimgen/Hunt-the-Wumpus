#ifndef CAVE_TYPES_HPP
#define CAVE_TYPES_HPP

#include <string>
#include <vector>

#define NONE -1

/*
 * Function: isNone
 * Description: Checks if the given variable has the value NONE.
 * Parameters:
 *    <T> (typename): The type of the variable to check. Can be inferred at
 *      compile time.
 *    toCheck (T): The variable to compare against NONE.
 * Returns (bool): True if toCheck has the value NONE.
 */
template<typename T> bool isNone(const T& toCheck) {
  return toCheck == (T)NONE;
}
/*
 * Function: notNone
 * Description: Checks if the given variable doesn't have the value NONE.
 * Parameters:
 *    <T> (typename): The type of the variable to check. Can be inferred at
 *      compile time.
 *    toCheck (T): The variable to compare against NONE.
 * Returns (bool): True if toCheck doesn't have the value NONE.
 */
template<typename T> bool notNone(const T& toCheck) {
  return toCheck != (T)NONE;
}


// Every action the player can perform, and their corresponding integer values.
enum Actions {
  UpDir,
  DownDir,
  LeftDir,
  RightDir,
  FireAction,
};


// Stores the location of a room on the board.
struct RoomPos {
  int row;
  int col;

  // Additional info to store if needed. Isn't used by basic implementations.
  int info = NONE;

public:
  /*
   * Function: Default contructor
   * Description: Constructs a RoomPos object with row and col set to NONE.
   */
  RoomPos();

  /*
   * Function: Constructor
   * Description: Initializes both row and col to the given value. Should be
   *    primarily used with NONE as the argument to show an initialization to
   *    empty.
   * Parameters:
   *    num (int): The number to initialize row and col to.
   */
  RoomPos(int num);

  /*
   * Function: Constructor
   * Description: Initialized a RoomPos object with the given row and col
   *    values.
   * Parameters:
   *    row (int): The value to assign to row
   *    col (int): The value to assign to col
   */
  RoomPos(int row, int col);

  /*
   * Function: Constructor
   * Description: Initialized a RoomPos object with the given row, col, and
   *    info values.
   * Parameters:
   *    row (int): The value to assign to row
   *    col (int): The value to assign to col
   *    info (int): The value to assign to info
   */
  RoomPos(int row, int col, int info);
  
  /*
   * Function: Equivilance Operator
   * Description: Compares two RoomPos objects to see if they're equal.
   * Parameters:
   *    other (RoomPos): The other RoomPos to compare this against.
   * Returns (bool): True if the two RoomPos objects share the same values,
   *    and false otherwise.
   */
  bool operator==(const RoomPos& other) const;
  bool operator!=(const RoomPos& other) const;

  /*
   * Function: empty
   * Description: Checks to see if the default constructor was used to create
   *    this, or if it was initialized with a value of NONE for row or col.
   * Returns: A bool that evaluates to whether RoomPos stores useful
   *    information.
   */
  bool empty() const;
};


// The game display modes and their corresponding integer values.
enum GameModeTypes {
  NormalMode,
  DebugMode,
};

#endif
