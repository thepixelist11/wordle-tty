#pragma once

#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "../tile/tile.h"
#include "../wordlists/wordlist.h"

std::string getRandomWord(const std::vector<std::string> wordlist);

struct CharState {
  char c;
  tileType state;
};

class Grid {
 public:
  uint8_t maxGuesses = 6;
  uint8_t wordlength = 5;
  std::vector<CharState> charStates;
  std::vector<Tile> tiles;
  bool didWin;

  Grid(uint8_t maxGuesses, std::vector<std::string> wordlist);
  Grid(uint8_t maxGuesses, wordlistID, std::string argv0);

  std::string getWord() { return word; }
  std::vector<std::string> getWordList() { return wordlist; }
  int getCurrentGuesses() { return currentGuesses; }
  void updateTileTypes(int guess, std::string& word);

  void enterWord(std::string word);

 private:
  std::string word;
  std::vector<std::string> wordlist;
  int currentGuesses = 0;

  void createGrid();

  bool isValidWord(std::string word);
};

std::ostream& operator<<(std::ostream& out, Grid& grid);
