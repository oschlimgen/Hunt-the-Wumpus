#include "gameUpdatePointer.hpp"

// #include <utility>


void GameUpdate::pointer::incrementReference() {
  if(refCount) {
    (*refCount)++;
  }
}

void GameUpdate::pointer::dereference() {
  if(refCount) {
    (*refCount)--;
    if(*refCount < 1) {
      delete ptr;
      delete refCount;
      ptr = nullptr;
      refCount = nullptr;
    }
  }
}

GameUpdate::pointer::pointer() : ptr(nullptr), refCount(nullptr),
    nextUpdate(nullptr) {}
GameUpdate::pointer::pointer(GameUpdate* update) : ptr(update),
    refCount(nullptr), nextUpdate(nullptr) {
  if(ptr) {
    refCount = new int(1);
  }
}

GameUpdate::pointer::pointer(const pointer& other) : ptr(other.ptr),
    refCount(other.refCount), nextUpdate(other.nextUpdate) {
  incrementReference();
}
GameUpdate::pointer& GameUpdate::pointer::operator=(const pointer& other) {
  if(this != &other) {
    dereference();
    deleteNext();

    ptr = other.ptr;
    refCount = other.refCount;
    incrementReference();

    // Only shallow copy the rest of the list
    nextUpdate = other.nextUpdate;
  }
  return *this;
}

GameUpdate::pointer::pointer(pointer&& other) : ptr(other.ptr),
    refCount(other.refCount), nextUpdate(other.nextUpdate) {
  other.ptr = nullptr;
  other.refCount = nullptr;
  other.nextUpdate = nullptr;
}
GameUpdate::pointer& GameUpdate::pointer::operator=(pointer&& other) {
  if(this != &other) {
    dereference();
    deleteNext();

    ptr = other.ptr;
    refCount = other.refCount;
    nextUpdate = other.nextUpdate;

    other.ptr = nullptr;
    other.refCount = nullptr;
    other.nextUpdate = nullptr;
  }
  return *this;
}

GameUpdate::pointer::~pointer() {
  dereference();
  // Not deleting nextUpdate!! Must be managed at a higher level.
}

GameUpdate& GameUpdate::pointer::operator*() {
  return *ptr;
}
const GameUpdate& GameUpdate::pointer::operator*() const {
  return *ptr;
}
GameUpdate* GameUpdate::pointer::operator->() {
  return ptr;
}
const GameUpdate* GameUpdate::pointer::operator->() const {
  return ptr;
}

GameUpdate::pointer::operator bool() const {
  return ptr || nextUpdate;
}

bool GameUpdate::pointer::empty() const {
  return ptr == nullptr;
}

bool GameUpdate::pointer::operator==(const pointer& other) const {
  return ptr == other.ptr;
}

GameUpdate::pointer& GameUpdate::pointer::operator++() {
  dereference();

  if(nextUpdate) {
    ptr = nextUpdate->ptr;
    refCount = nextUpdate->refCount;
    incrementReference();

    nextUpdate = nextUpdate->nextUpdate;

  } else {
    ptr = nullptr;
    refCount = nullptr;
  }
  return *this;
}
GameUpdate::pointer GameUpdate::pointer::operator++(int) {
  operator++();
  return nullptr;
}

void GameUpdate::pointer::moveToNext() {
  dereference();

  if(nextUpdate) {
    ptr = nextUpdate->ptr;
    refCount = nextUpdate->refCount;
    nextUpdate->ptr = nullptr;
    nextUpdate->refCount = nullptr;

    // Have to save nextUpdate to delete it after replacement
    pointer* next = nextUpdate;
    nextUpdate = next->nextUpdate;
    // next should be set to nullptr, but must free pointer class memory
    delete next;

  } else {
    ptr = nullptr;
    refCount = nullptr;
  }
}

GameUpdate::pointer GameUpdate::pointer::next() {
  if(nextUpdate) {
    return *nextUpdate;
  }
  return nullptr;
}
GameUpdate::pointer* GameUpdate::pointer::nextPointer() {
  return nextUpdate;
}


void GameUpdate::pointer::setNext(const pointer& next) {
  // This will shallow copy the rest of the linked list
  nextUpdate = new pointer(next);
}
void GameUpdate::pointer::setNext(pointer* const next) {
  nextUpdate = next;
}

void GameUpdate::pointer::append(const pointer& update){
  if(nextUpdate) {
    nextUpdate->append(update);
  } else {
    if(ptr) {
      setNext(update);

    } else {
      // This object is empty, so transfer update data
      ptr = update.ptr;
      refCount = update.refCount;
      nextUpdate = update.nextUpdate;
      incrementReference();
    }
  }
}

void GameUpdate::pointer::insertNext(const pointer& update) {
  pointer* next = nextUpdate;
  nextUpdate = nullptr;

  setNext(update);

  if(next) {
    append(*next);
    delete next;
  }
}

void GameUpdate::pointer::deleteNext() {
  if(nextUpdate) {
    nextUpdate->deleteNext();
    delete nextUpdate;
    nextUpdate = nullptr;
  }
}
