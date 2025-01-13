#ifndef GAME_UPDATE_HPP
#define GAME_UPDATE_HPP

#include "htwTypes.hpp"

class Player;
class Event;
class Trigger;

class GameUpdate {
public:
  enum UpdateType {
  /* Handled by the Game object */
    // No additional information needed
    ForceGameEnd,
    // No additional information needed
    ForceUpdateEnd,
    // targetTrigger: contains text describing the win
    WinGame,
    // targetTrigger: contains text describing the loss
    LoseGame,
    // info: The game mode to switch into
    SetGameMode,
    // info == 1: The user will not be prompted for a turn action
    // info == NONE: The player's action will be used
    WaitForMove,
    // info == 1: The user will not be prompted for a turn action
    // info == NONE: The player's action will be used
    WaitTurnCycle,
    // No additonal information needed
    EndWait,
    // No additional information needed
    BeginConditional,
    // No additional information needed
    EndConditional,
    // No additional information needed
    RefreshBoardDisplay,
    // targetTrigger: contains the text to display
    DisplayText,


  /* Handled by the Cave object */
    // targetPlayer: The active player, to convert keys
    PromptPlayerAction,
    // info == NONE || info == 0: Turn action prompts won't be executed OR
    // info == 1: Re-enables executing turn action prompts
    SetPlayerActionEnabled,
    // targetPlayer: The active player, to convert keys
    // targetTrigger: trigger to set the direction of
    PromptDirection,
    // targetEvent: a new Event to place OR
    // targetTrigger: a new Trigger to place
    CreateObject,
    // targetEvent: event to move OR
    // targetTrigger: trigger to move OR
    // targetPlayer: player to move
    // ------------ AND ------------
    // room: where to put it OR
    // info: direction to move it OR
    // info == NONE (for triggers & players): move in the stored direction
    // ---------- OPTIONAL ---------
    // targetEvent (for Player only): text to display on failure to move
    MoveObject,
    // targetEvent: event to move OR
    // targetTrigger: trigger to move OR
    // targetPlayer: player to move
    // ------------ AND ------------
    // info == 0: move to a random location (not necessarily empty) OR
    // info == 1: move to a random EMPTY location OR
    // info == 2: move to an adjacent room (not necessarily empty) OR
    // info == 3: move to an EMPTY adjacent room
    MoveObjectRandom,
    // targetEvent: event to remove OR
    // targetTrigger: trigger to remove
    DestroyObject,
    // targetPlayer: player to add Item to,
    // targetEvent: event to query for Item,
    // ---------- OPTIONAL ---------
    // info: number of the item to add
    PickupItem,
    // targetPlayer: player to remove the Item from
    // targetEvent: contains the text name of the item to check for
    // ---------- OPTIONAL ---------
    // info: number of item to remove 
    RemoveItem,
    // targetPlayer: player to check for item
    // targetEvent: contains the text name of the item to check for
    // ---------- OPTIONAL ---------
    // info: number of item to check for OR
    // info == 0: checks if player has none of the item to proceed
    ItemConditional,
    // info: boolean value to set enabled to
    SetObjectEnabled,
    // targetTrigger: trigger for the Event to react to OR
    // targetPlayer: player for the Event to react to
    // ------------ AND ------------
    // targetEvent: contains the text name of the event to trigger
    TriggerEvent,
  };
  
  enum class WhatToDelete {
    Nothing,
    TargetPlayer,
    TargetEvent,
    TargetTrigger,
  };

  // Combination of shared ptr + linked list
  class pointer;

private:
  int type; // Should be defined for each instance

  int info = NONE;

  Player* targetPlayer = nullptr;
  Event* targetEvent = nullptr;
  Trigger* targetTrigger = nullptr;

  WhatToDelete toDelete = WhatToDelete::Nothing;

public:
  /*
   * Function: Minimal Constructor
   * Description: All GameUpdate objects should be initialized with an
   *    integer corresponding to a value in UpdateType. This constructor
   *    initialzes a GameUpdate object with this type and sets other members
   *    to NONE or null.
   */
  GameUpdate(int type);

  /*
   * Functions: Single-Member Constructors
   * Description: Initializes a GameUpdate object with a type and one of the
   *    members to contain information about how to execute the update.
   */
  GameUpdate(int type, int info);
  GameUpdate(int type, Event* event);
  GameUpdate(int type, Trigger* trigger);
  GameUpdate(int type, const std::string& message);

  /*
   * Functions: Multiple-Member Constructors
   * Description: Initializes a GameUpdate object with a type and two or more
   *    of the members to contain information about how to execute the update.
   */
  GameUpdate(int type, const std::string& message, int info);

  GameUpdate(int type, Event* event, int info);
  GameUpdate(int type, Event* event, RoomPos room);
  GameUpdate(int type, Event* event, WhatToDelete toDelete);
  GameUpdate(int type, Trigger* trigger, int info);
  GameUpdate(int type, Trigger* trigger, RoomPos room);
  GameUpdate(int type, Trigger* trigger, WhatToDelete toDelete);

  GameUpdate(int type, Player* player, Event* event);
  GameUpdate(int type, Player* player, Event* event, int info);
  GameUpdate(int type, Player* player, Trigger* trigger);
  GameUpdate(int type, Player* player, Trigger* trigger, int info);

  GameUpdate(int type, Event* event, const std::string& message);
  GameUpdate(int type, Event* event, const std::string& message, int info);
  GameUpdate(int type, Trigger* trigger, const std::string& message);
  GameUpdate(int type, Trigger* trigger, const std::string& message, int info);
  
  /*
   * Function: Complete Constructor
   * Description: Takes each member as an argument (except for nextUpdate),
   *    and initializes a GameUpdate with the given values. To be used when
   *    none of the other constructors match.
   */
  GameUpdate(int type, Player* player, Event* event, Trigger* trigger,
      int info, WhatToDelete toDelete);

  /*
   * Functions: Copy constructor and copy assignment
   * Description: Frees the memory specified by toDelete, copies memory
   *    specified by other.toDelete, then transfers data from other.
   * Parameters:
   *    other (GameUpdate): The GameUpdate object to copy.
   */
  GameUpdate(const GameUpdate& other);
  GameUpdate& operator=(const GameUpdate& other);

  /*
   * Function: Destructor
   * Description: Frees only the memory specified by toDelete. Other memory
   *    should be managed externally.
   */
  ~GameUpdate();

  /*
   * Functions: Flexible getters
   * Description: Flexible direct access to class members is necessary here
   *    because all of the information stored may be needed to resolve the
   *    gameUpdate, and the information may be used in many unique ways that
   *    shouldn't be limited at this level of the program.
   * Returns: A copy of class members.
   */
  operator int() const;
  int getType() const;
  int getInfo() const;
  Player* getPlayer() const;
  Event* getEvent() const;
  Trigger* getTrigger() const;

  /*
   * Functions: has[Object]
   * Description: Checks if the update has a pointer to the given object type,
   *    and that it isn't supposed to be deleted.
   * Returns (bool): True if the pointer to the given object isn't null and
   *    toDelete isn't set to the given object type. Otherwise returns false.
   */
  bool hasPlayer() const;
  bool hasEvent() const;
  bool hasTrigger() const;
  
  /*
   * Function: getRoom
   * Description: Uses toDelete to deduce if a RoomPos object was passed to the
   *    constructor. If it was, this function returns it.
   * Returns (RoomPos): Returns the RoomPos object passed to the constructor if
   *    there was one. Otherwise, returns a RoomPos set to NONE.
   */
  RoomPos getRoom() const;

  /*
   * Function: getMessage
   * Description: Uses toDelete to deduce if a message string was passed to the
   *    constructor. If it was, this function returns it.
   * Returns (RoomPos): Returns the message string passed to the constructor if
   *    there was one. Otherwise, returns an empty string.
   */
  std::string getMessage() const;
};

#endif
