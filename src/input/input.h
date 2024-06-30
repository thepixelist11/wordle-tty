#pragma once

#include <string>
#include "../grid/grid.h"

class Input {
 public:
  Input(Grid g);

  std::string inputChars;
  Grid g;

  void addChar(int c);
};

std::ostream& operator<<(std::ostream& out, Input& input);
