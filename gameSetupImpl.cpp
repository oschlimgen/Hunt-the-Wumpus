#include "gameSetupImpl.hpp"

#include <iostream>
#include <random>
#include <ctime>

#include "game.hpp"
#include "goldEvent.hpp"
#include "batSwarm.hpp"
#include "bottomlessPit.hpp"
#include "wumpus.hpp"
#include "escapeRope.hpp"
#include "arrowEvent.hpp"



// Game implementation
int BasicSetup::randomNumber(const int max) {
  return rand() % max;
}

std::vector<int> BasicSetup::chooseUnique(const int maxNum, const int toChoose) {
  std::vector<int> chosen;
  for(int i = 0; i < toChoose; ++i) {
    int num = randomNumber(maxNum - chosen.size());
    int j = 0;
    for(; j < chosen.size(); ++j) {
      if(chosen.at(j) > num) {
        break;
      }
      // At this point, the number in chosen must be less than or equal to
      num++;
    }
    chosen.insert(chosen.begin() + j, num);
  }
  return chosen;
}

std::vector<int> BasicSetup::chooseUniqueUnordered(const int toChoose) {
  // Create a list of numbers from 0 to toChoose
  std::vector<int> nums(toChoose);
  for(int i = 0; i < toChoose; ++i) {
    nums.at(i) = i;
  }

  // Swap the item at each list position to a new random position to randomize
  for(int i = 0; i < toChoose; ++i) {
    int n = randomNumber(toChoose);
    std::swap(nums.at(i), nums.at(n));
  }
  return nums;
}



int BasicSetup::promptUserInput(const std::string& prompt, const int minValue,
    const int maxValue) {
  int choice = 0;
  std::string response;
  std::cout << prompt;

  while(std::getline(std::cin, response)) {
    // Try to convert response to an integer
    try {
      choice = std::stoi(response);
    } catch(std::invalid_argument&) {
      std::cout << "You must enter a number. Try again: ";
      continue;
    } catch (std::out_of_range&) {
      std::cout << "Your number must be within the range. Try again: ";
      continue;
    }

    // Check bounds
    if(choice < minValue || choice > maxValue) {
      std::cout << "Your number must be within the range. Try again: ";
      continue;
    }

    // If it reaches this point, the response is valid
    break;
  }
  return choice;
}

int BasicSetup::promptUserInputWithRange(const std::string& prompt,
      const int minValue, const int maxValue) {
  // appends "(min - max): " to the prompt
  std::string modifiedPrompt = prompt + " (" + std::to_string(minValue) +
      " - " + std::to_string(maxValue) + "): ";
  return promptUserInput(modifiedPrompt, minValue, maxValue);
}



BasicSetup::BasicSetup(const Event* defaultEvent) : GameSetup(defaultEvent->clone()) {
  srand(time(nullptr));
}

void BasicSetup::promptConfigurations() {
  constexpr int minCaveSize = 4;
  constexpr int maxCaveSize = 30;
  constexpr int numGameModes = 2;

  dimensions.resize(2);
  dimensions[1] = promptUserInputWithRange("Enter the game board width",
      minCaveSize, maxCaveSize);
  dimensions[0] = promptUserInputWithRange("Enter the game board height",
      minCaveSize, maxCaveSize);

  gameMode = promptUserInput("Would you like to play in debug mode? "
      "(1-yes, 0-no): ", 0, numGameModes - 1);
}

void BasicSetup::init() {
  const int height = dimensions.at(0);
  const int width = dimensions.at(1);

  addEventType<Wumpus>();
  addEventType<BatSwarm>(2);
  addEventType<BottomlessPit>(2);
  addEventType<ArrowEvent>(2);
  addEventType<EscapeRope>();

  const int numRooms = height * width;
  if(numRooms < events.size()) {
    throw std::out_of_range("The number of rooms in the cave must be greater "
        "than or equal to the number of events present. Events present: " +
        std::to_string(events.size()));
  }
  std::vector<int> rooms = BasicSetup::chooseUnique(numRooms, events.size());
  std::vector<int> pairings = BasicSetup::chooseUniqueUnordered(events.size());

  auto roomLoc = pairings.begin();
  RoomPos ropeLocation;
  for(Event* ev : events) {
    // Place the event using the random rooms and pairings
    int roomNum = rooms.at(*roomLoc);
    ev->setLocation({ roomNum / width, roomNum % width });
    ++roomLoc;
    
    // Record the location of the escape rope for later use
    if(ev->name() == EscapeRope::eventName) {
      ropeLocation = ev->getLocation();
    }
  }
  
  // Place Gold Event at least half the board away
  const int minDist = (int)ceil((double)(height +
      width) / 4.0);
  // Find each room the gold could be placed in
  std::vector<int> possibleGoldLoc;
  std::vector<int>::iterator filledRooms = rooms.begin();
  for(int rw = 0; rw < height; ++rw) {
    for(int col = 0; col < width; ++col) {
      // Don't consider the room as a possible location if it has another event
      if(filledRooms != rooms.end() && *filledRooms == rw * width + col) {
        filledRooms++;
        continue;
      }
      // Make sure the gold location is far enough away from the players
      if(abs(rw - ropeLocation.row) + abs(col - ropeLocation.col) > minDist) {
        possibleGoldLoc.push_back(rw * width + col);
      }
    }
  }
  
  // Add and find the gold event in the list of events
  addEventType<GoldEvent>();
  Event* gold = nullptr;
  for(Event* ev : events) {
    // Find gold in event in case it has been modded and overriden
    if(ev->name() == GoldEvent::eventName) {
      gold = ev;
    }
  }
  // Place the gold based on its possible locations
  int goldRoom = possibleGoldLoc.at(randomNumber(possibleGoldLoc.size()));
  gold->setLocation({ goldRoom / width, goldRoom % width });

  // Place players at the escape rope
  for(Player* pl : players) {
    pl->setLocation(ropeLocation);
  }
}

