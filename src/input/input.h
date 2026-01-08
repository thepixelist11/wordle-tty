#pragma once

#include "../grid/grid.h"
#include <string>

class Input {
  public:
    Input(Grid g);

    std::string inputChars;
    Grid        g;

    void addChar(int c);
};

std::ostream& operator<<(std::ostream& out, Input& input);
