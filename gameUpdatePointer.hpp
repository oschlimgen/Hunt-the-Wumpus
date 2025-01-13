#ifndef GAME_UPDATE_POINTER_HPP
#define GAME_UPDATE_POINTER_HPP

#include "gameUpdate.hpp"


class GameUpdate::pointer {
private:
  GameUpdate* ptr;
  int* refCount;
  pointer* nextUpdate;

  /*
   * Function: incrementReference
   * Description: Adds one to the reference count because a new instance of the
   *    pointer has been created.
   */
  void incrementReference();
  /*
   * Function: dereference
   * Description: Subtracts one from the reference count then deletes the
   *    pointer if it has no references.
   */
  void dereference();

public:
  /*
    * Function: Default Constructor
    * Description: Initializes the pointer to nullptr.
    */
  pointer();
  /*
    * Function: Constructor
    * Description: Initialized the pointer to be equal to the passed pointer.
    *    This object will then take care of memory management for the
    *    GameUpdate object, so the pointer passed should be discarded.
    */
  pointer(GameUpdate* update);

  /*
    * Functions: Copy Constructor/Assignment
    * Description: Deep copies the GameUpdate object referenced by other.
    */
  pointer(const pointer& other);
  pointer& operator=(const pointer& other);

  /*
    * Functions: Move Constructor/Assignment
    * Description: Transfers the underlying pointer from one pointer object
    *    to the other.
    */
  pointer(pointer&& other);
  pointer& operator=(pointer&& other);

  /*
    * Function: Destructor
    * Description: Deletes the underlying pointer, freeing the memory
    *    allocated to the GameUpdate object.
    */
  ~pointer();

  /*
    * Functions: Dereference Operators
    * Description: These functions return a GameUpdate reference for access
    *    to the GameUpdate object stored by the pointer.
    */
  GameUpdate& operator*();
  const GameUpdate& operator*() const;
  GameUpdate* operator->();
  const GameUpdate* operator->() const;

  /*
    * Function: Implicit Boolean Conversion
    * Description: Evaluates to a boolean that is false if ptr and nextUpdate
    *    are both null, and true otherwise.
    */
  operator bool() const;

  /*
    * Function: empty
    * Description: Checks if the GameUpdate exists
    * Returns (bool): Evaluates to true iff ptr is null.
    */
  bool empty() const;

  /*
    * Function: Equivilancy Operator
    * Description: Evaluates to true if the two underlying pointers hold the
    *    same memory adress. In practice, this should only happen when
    *    comparing a pointer object to itself or nullptr. Otherwise it
    *    evaluates to false.
    */
  bool operator==(const pointer& other) const;

  /*
    * Function: Increment Operator
    * Description: Takes on the information of nextUpdate then deletes it.
    *    Allows for memory-safe forward iteration of the linked list. Note
    *    that because nextUpdate is deleted, the list may only be iterated
    *    once in this way.
    * Returns (pointer): A default-constructed pointer that contains no  
    *    information.
    */
  pointer& operator++();
  pointer operator++(int);

  /*
   * Function: moveToNext
   * Description: Takes on the information of nextUpdate then deletes it.
   *    Allows for memory-safe forward iteration of the linked list. Note
   *    that because nextUpdate is deleted, the list may only be iterated
   *    once in this way.
   * Returns (pointer): A default-constructed pointer that contains no  
   *    information.
   */
  void moveToNext();
  
  /*
   * Function: next
   * Description: Gets the next GameUpdate in the list if there is one. The
   *    returned object will have the same next members.
   * Returns (pointer): Returns the nextUpdate member if it exists, and
   *    returns a pointer object that evaluates to null otherwise.
   */
  pointer next();
  /*
   * Function: next
   * Description: Gets the pointer to the next GameUpdate in the list.
   * Returns (pointer): Returns the nextUpdate member.
   */
  pointer* nextPointer();
  
  /*
   * Function: setNext
   * Description: Sets the next update to execute after finishing this one.
   * Parameters:
   *    next (pointer): A pointer to the GameUpdate to add to the queue.
   */
  void setNext(const pointer& next);
  void setNext(pointer* next);

  /*
   * Function: append
   * Description: Adds the given update pointer to the end of the linked list
   *    of update pointers.
   * Parameters:
   *    update (pointer): The pointer to the GameUpdate to add.
   */
  void append(const pointer& update);

  /*
   * Function: insertNext
   * Description: Inserts the given update pointer at the next position of the
   *    linked list of update pointers.
   * Parameters:
   *    update (pointer): The poitner to the GameUpdate to insert
   */
  void insertNext(const pointer& update);

  /*
   * Function: deleteNext
   * Description: Frees all memory in the linked list after this object.
   * Effects: Safely de-allocates memory for the rest of the linked list.
   */
  void deleteNext();
};

#endif
