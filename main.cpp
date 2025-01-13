/*
 * Author: Owen Schlimgen
 * Description: This program runs the game Hunt the Wumpus. Hunt the Wumpus is
 *    a text-based game set in the cave of the Wumpus, a gold-hoarding monster.
 *    The player enters the cave as an adventurer in search of the Wumpus'
 *    gold. The player has to avoid the traps and obstacles in the cave using
 *    their percepts for being close to an event. If the player successfully
 *    finds the gold and escapes the cave, or kills the Wumpus, then they win.
 *    The game also comes with a mod loader, which can add a wide variety of
 *    features to the base game.
 */

#include <iostream>
#include <filesystem>

#include "game.hpp"
#include "loadMods.hpp"

#define MOD_DIR "mods"


int main()
{
  // Load mods from the mod directory
  LoadMods mods(MOD_DIR);

  // Get the GameSetup and Cave objects
  GameSetup* setup = mods.getSetup();
  Cave* cave = mods.getCave();

  // Construct Game object
  Game g(setup, cave);

  //Play the game
  g.playGame();

  return 0;
}
