#include "input.h"
#include "../keycodes/keycodes.h"

Input::Input(Grid g) : g(g) {};

std::ostream& operator<<(std::ostream& out, Input& input) {
  std::string outputChars = "";
  for (int i = 0; i < input.g.wordlength; i++) {
    if (i < input.inputChars.length()) {
      outputChars += input.inputChars[i];
      outputChars += ' ';
    } else {
      outputChars += '_';
      outputChars += ' ';
    }
  }
  return out << outputChars;
}

void Input::addChar(int c) {
  if (isValidKey(c)) {
    if (c == KEY_BS) {
      if (inputChars.length() > 0) {
        inputChars.erase(inputChars.length() - 1);
      }
    } else {
      if (inputChars.length() < g.wordlength) {
        inputChars += char(c);
      }
    }
  }
}
