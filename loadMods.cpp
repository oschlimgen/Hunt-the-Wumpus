#include "loadMods.hpp"

#include <cstdint>

#include "defaultEvent.hpp"
#include "gameSetupImpl.hpp"
#include "playerImpl.hpp"
#include "caveImpl.hpp"



void LoadMods::loadDefaultEvent() {
  for(HANDLE_TYPE& h : handles) {
    Event* (*createDefaultEvent)() = nullptr;
    createDefaultEvent = reinterpret_cast<Event* (*)()>(
        GetFunctionPointer(h, GET_DEFAULT_EVENT));
    if(createDefaultEvent) {
      if(defaultEvent != nullptr) {
        throw std::runtime_error("Duplicate function getDefaultEvent() "
            "in mods folder. Please check mod compatability.");
      }
      defaultEvent = createDefaultEvent();
      std::cout << "Default Event Mod Loaded." << std::endl;
    }
  }

  if(!defaultEvent) {
    defaultEvent = new DefaultEvent();
  }
}

void LoadMods::loadSetup() {
  for(HANDLE_TYPE& h : handles) {
    GameSetup* (*createSetup)(const Event*) = nullptr;
    createSetup = reinterpret_cast<GameSetup* (*)(const Event*)>
        (GetFunctionPointer(h, GET_GAME_SETUP));
    if(createSetup) {
      if(setup != nullptr) {
        throw std::runtime_error("Duplicate function getSetup() "
            "in mods folder. Please check mod compatability.");
      }
      setup = createSetup(defaultEvent);
      std::cout << "GameSetup Mod Loaded." << std::endl;
    }
  }
  if(!setup) {
    setup = new BasicSetup(defaultEvent);
  }
  
  // Ask user for setup configurations
  setup->promptConfigurations();
}

void LoadMods::loadPlayers() {
  std::vector<Player*> players;

  for(HANDLE_TYPE& h : handles) {
    Player* (*createPlayer)(const GameSetup*) = nullptr;
    std::vector<Player*> (*createPlayersList)(const GameSetup*) = nullptr;

    createPlayer = reinterpret_cast<Player* (*)(const GameSetup*)>(
        GetFunctionPointer(h, GET_PLAYER));
    createPlayersList = reinterpret_cast
        <std::vector<Player*> (*)(const GameSetup*)>(
        GetFunctionPointer(h, GET_PLAYERS_LIST));
    
    if(createPlayer) {
      Player* pl = createPlayer(setup);
      players.push_back(pl);
      std::cout << "Player Mod Loaded." << std::endl;

    } else if(createPlayersList) {
      std::vector<Player*> pls = createPlayersList(setup);
      players.insert(players.end(), pls.begin(), pls.end());
      std::cout << "Players Mod Loaded." << std::endl;
    }
  }

  for(Player*& pl : players) {
    setup->addPlayer(pl);
  }
  if(players.size() == 0) {
    Player* pl = new BasicPlayer();
    setup->addPlayer(pl);
  }
}

void LoadMods::loadEvents() {
  std::vector<Event*> events;

  for(HANDLE_TYPE& h : handles) {
    Event* (*createEvent)(const GameSetup*) = nullptr;
    std::vector<Event*> (*createEventsList)(const GameSetup*) = nullptr;

    createEvent = reinterpret_cast<Event* (*)(const GameSetup*)>(
        GetFunctionPointer(h, GET_EVENT));
    createEventsList = reinterpret_cast<std::vector<Event*> (*)(const GameSetup*)>(
        GetFunctionPointer(h, GET_EVENTS_LIST));

    if(createEvent) {
      Event* ev = createEvent(setup);
      events.push_back(ev);
      std::cout << "Event Mod Loaded." << std::endl;

    } else if(createEventsList) {
      std::vector<Event*> evs = createEventsList(setup);
      events.insert(events.end(), evs.begin(), evs.end());
      std::cout << "Events Mod Loaded." << std::endl;
    }
  }

  for(Event*& ev : events) {
    setup->addEvent(ev);
  }
  // Initialize setup with relevant values
  setup->init();
}

void LoadMods::loadCave() {
  for(HANDLE_TYPE& h : handles) {
    Cave* (*createCave)(const GameSetup*) = nullptr;
    createCave = reinterpret_cast<Cave* (*)(const GameSetup*)>(
      GetFunctionPointer(h, GET_CAVE));
    if(createCave) {
      if(cave != nullptr) {
        throw std::runtime_error("Duplicate function getCave() "
            "in mods folder. Please check mod compatability.");
      }
      cave = createCave(setup);
      std::cout << "Cave Mod Loaded." << std::endl;
    }
  }
  
  if(!cave) {
    cave = new BasicCave(setup);
  }
  setup->clearEvents();
}



LoadMods::LoadMods() : defaultEvent(nullptr), setup(nullptr), cave(nullptr) {}

LoadMods::LoadMods(const std::string& dir) : defaultEvent(nullptr),
    setup(nullptr), cave(nullptr) {
  loadModHandles(dir);
  loadObjs();
}

LoadMods::~LoadMods() {
  delete defaultEvent;
  delete setup;
  delete cave;

  for(HANDLE_TYPE& h : handles) {
    CloseShared(h);
  }
}

GameSetup* LoadMods::getSetup() {
  return setup;
}
Cave* LoadMods::getCave() {
  return cave;
}

void LoadMods::loadModHandles(const std::string& dir) {
  std::filesystem::directory_iterator modFiles(dir);

  for(const auto& entry : modFiles) {
    if(entry.is_regular_file() && entry.path().extension() ==
        SHARED_EXTENSION) {
      HANDLE_TYPE h = LoadShared(entry.path().string().c_str() OPEN_FLAG);
      if(h) {
        handles.push_back(h);
        std::cout << "Loaded file: " << entry.path().string() << std::endl;
      } else {
        std::cout << "Mod file found, but failed to load. Path: " <<
            entry.path().string() << std::endl;
      }
    }
  }
}

void LoadMods::loadObjs() {
  // Initialize the default event, to be used by setup and events
  loadDefaultEvent();

  // Initialize main setup variables in case it affects player setup
  loadSetup();

  // Initialize players before events in case the events use the number of
  // players in the game.
  loadPlayers();

  // Finally initialize the events in setup (last component of GameSetup)
  loadEvents();

  // Use setup to initialize Cave
  loadCave();
}
