#include "htwTypes.hpp"



RoomPos::RoomPos() : row(NONE), col(NONE) {}
RoomPos::RoomPos(const int num) : row(num), col(num) {}
RoomPos::RoomPos(const int row, const int col) : row(row), col(col) {}
RoomPos::RoomPos(const int row, const int col, const int info) :
    row(row), col(col), info(info) {}

bool RoomPos::operator==(const RoomPos& other) const {
  return (row == other.row && col == other.col && info == other.info);
}
bool RoomPos::operator!=(const RoomPos& other) const {
  return !operator==(other);
}

bool RoomPos::empty() const {
  return (row < 0 || col < 0);
}
