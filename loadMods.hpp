/*
 * The following function names and signatures are searched for:
 *    Event* getDefaultEvent()
 *    GameSetup* getSetup(const Event* default)
 *    Player* getPlayer(const GameSetup*)
 *    std::vector<Player*> getPlayers(const GameSetup*)
 *    Event* getEvent(const GameSetup*)
 *    std::vector<Event*> getEvents(const GameSetup*)
 *    Cave* getCave(const GameSetup*)
 */

#include <vector>
#include <filesystem>

#include "dlSpDef.hpp"
#include "gameSetup.hpp"
#include "cave.hpp"

#include <iostream>

#define GET_DEFAULT_EVENT "getDefaultEvent"
#define GET_DEFAULT_EVENT_FUNC Event* getDefaultEvent()

#define GET_GAME_SETUP "getSetup"
#define GET_GAME_SETUP_FUNC GameSetup* getSetup(const Event* defaultEvent)

#define GET_PLAYER "getPlayer"
#define GET_PLAYER_FUNC Player* getPlayer(const GameSetup* setup)
#define GET_PLAYERS_LIST "getPlayers"
#define GET_PLAYERS_LIST_FUNC std::vector<Player*> getPlayers(const GameSetup* setup)

#define GET_EVENT "getEvent"
#define GET_EVENT_FUNC Event* getEvent(const GameSetup* setup)
#define GET_EVENTS_LIST "getEvents"
#define GET_EVENTS_LIST_FUNC std::vector<Event*> getEvents(const GameSetup* setup)

#define GET_CAVE "getCave"
#define GET_CAVE_FUNC Cave* getCave(const GameSetup* setup)

class LoadMods {
private:
  std::vector<HANDLE_TYPE> handles;

  Event* defaultEvent;
  GameSetup* setup;
  Cave* cave;

  /*
   * Function(s): Load [object]
   * Description: Attempt to locate functions with specific names for loading
   *    mods into the game. Mods must have the given function names with the
   *    proper types to be loaded properly. Function names and signatures are
   *    given at the top of this file.
   */
  void loadDefaultEvent();
  void loadSetup();
  void loadPlayers();
  void loadEvents();
  void loadCave();

public:
  /*
   * Function: Default Constructor
   * Description: Sets all stored pointers to nullptr.
   */
  LoadMods();
  /*
   * Function: Constructor
   * Description: Looks for mod files in the given directory then loads them
   *    by calling loadModHandles and loadObjs.
   * Effects: Loads the mods in the given directory.
   */
  LoadMods(const std::string& dir);

  /*
   * The class has internal pointers and no reasonable way or reason to copy
   *    them, so the copy and move functions are deleted.
   */
  LoadMods(const LoadMods&) = delete;
  LoadMods(LoadMods&&) = delete;
  LoadMods& operator=(const LoadMods&) = delete;
  LoadMods& operator=(LoadMods&&) = delete;

  /*
   * Function: Destructor
   * Description: Deletes each of the pointers stored by this class, and closes
   *    each shared object file loaded.
   * Effects: Deletes pointers stored by the object.
   */
  ~LoadMods();

  /*
   * Function: getSetup
   * Description: Simple getter for the GameSetup object. The pointer returned
   *    by this function shouldn't be deleted, as the destructor will do so.
   * Returns (GameSetup*): A pointer to the GameSetup object.
   */
  GameSetup* getSetup();
  /*
   * Function: getCave
   * Description: Simple getter for the Cave object. The pointer returned by
   *    this function shouldn't be deleted, as the destructor will do so.
   * Returns (Cave*): A pointer to the Cave object.
   */
  Cave* getCave();

  /*
   * Function: loadModHandles
   * Description: Searches the given directory for shared object files, then
   *    loads their contents using operating-specific functions. Stores their
   *    pointers/handles in a vector.
   * Parameters:
   *    dir (string): Path to the directory containing shared object mod files.
   * Effects: Loads shared objects and saves their handles. The should be
   *    unloaded later for memory safeness.
   */
  void loadModHandles(const std::string& dir);

  /*
   * Function: loadObjs
   * Description: Calls each of the private member load functions in
   *    succession, searching each shared object file for functions of the
   *    expected name and signature. Calls the functions to get pointers to
   *    the given objects.
   * Effects: Sets this class's pointers to objects from the mod files or to
   *    the default objects.
   */
  void loadObjs();
};
