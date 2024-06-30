#pragma once

#include <iostream>

enum tileType {
  TYPE_NONE,
  TYPE_GREY,
  TYPE_GREEN,
  TYPE_YELLOW,
};

class Tile {
 public:
  Tile();
  Tile(char let);

  tileType setType(tileType newType);

  tileType type = TYPE_NONE;
  char let = ' ';
};
