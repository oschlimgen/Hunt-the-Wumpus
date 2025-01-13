#ifndef WANDERING_WUMPUS_HPP
#define WANDERING_WUMPUS_HPP

#include "gameSetup.hpp"
#include "wumpus.hpp"

class WanderingWumpus : public Wumpus {
private:
  static constexpr int createBabiesAfter = 10;
  static constexpr int babiesToCreate = 3;

  int roundCount = 0;

public:
  /*
   * Function: Constructor
   * Description: Creates the Wumpus event with the given defaultEvent.
   */
  WanderingWumpus(Event* defaultEvent);

  /*
   * Function: clone
   * Description: Creates a new WanderingWumpus event, as opposed to the
   *    standard Wumpus event.
   * Returns (Event*): A newly allocated WanderingWumpus event.
   * Effects: Allocates memroy which must be deleted.
   */
  Event* clone() const override;

  /*
   * Function: turnUpdate
   * Description: Causes the Wumpus to move one room to a new empty room
   *    at the beginning of each round. Increments a turn counter, and spawns
   *    baby Wumpus when it hits a predetermined round.
   * Parameters:
   *    active (Player*): The player whose turn it is.
   *    round (bool): True if it's the first turn of the round.
   * Returns (GameUpdate::pointer): An update that moves the Wumpus to a new
   *    empty adjacent room. If the turn counter reaches the predetermined
   *    round, returns an update that creates three baby Wumpus events in
   *    random empty rooms and displays a message about their creation to each
   *    player.
   */
  GameUpdate::pointer turnUpdate(Player* active, bool round) override;
};


/*
 * Function: getEvent
 * Description: The function called by the base game during setup to load the
 *    event into the game.
 * Parameters:
 *    setup (GameSetup): The GameSetup object containing parameters entered
 *      by the user.
 * Returns (Event*): A newly allocated WanderingWumpus event.
 * Effects: Allocates memory which must be deleted.
 */
extern "C" Event* getEvent(const GameSetup* setup);


#endif