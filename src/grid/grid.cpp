#include "grid.h"
#include <algorithm>
#include <cstring>
#include "../exepath/exepath.h"

std::string getRandomWord(const std::vector<std::string> wordlist) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, wordlist.size() - 1);
  int i = distr(gen);
  return wordlist[i];
}

int binarySearch(std::vector<std::string> strings, std::string x) {
  int left = 0;
  int right = strings.size() - 1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    auto s1 = x;
    auto s2 = strings[mid];
    s1.erase(std::remove(s1.begin(), s1.end(), char(13)), s1.end());
    s2.erase(std::remove(s2.begin(), s2.end(), char(13)), s2.end());
    if (strcmp(s1.c_str(), s2.c_str()) == 0)
      return mid;
    if (x > strings[mid]) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return -1;
}

void Grid::createGrid() {
  auto longestWord =
      std::max_element(wordlist.begin(), wordlist.end(),
                       [](const auto& a, const auto& b) { return a.size() < b.size(); });
  wordlength = (*longestWord).length();
  word = getRandomWord(wordlist);
  for (int i = 0; i < wordlength - word.length(); i++) {
    word += ' ';
  }

  for (size_t i = 0; i <= wordlength * maxGuesses; i++) {
    tiles.push_back(Tile());
  }
}

Grid::Grid(uint8_t maxGuesses, std::vector<std::string> wordlist)
    : maxGuesses(maxGuesses), wordlist(wordlist), tiles() {
  createGrid();
}
Grid::Grid(uint8_t maxGuesses, wordlistID wordlistid, std::string argv0)
    : maxGuesses(maxGuesses), tiles() {
  switch (wordlistid) {
    case CLASSIC:
      wordlist = wordlist_commonWords;
      break;
    case FULL:
      wordlist = wordlist_allWords;
      break;
    case ALPHA:
      wordlist = wordlist_genAlphaSlang;
      break;
    case INSANE:
      auto exePath = getExecutablePath();
      std::string finalPath =
          exePath.substr(0, exePath.find_last_of('/')) + "/allWords.txt";
      wordlist = wordlistFromFile(finalPath);
      break;
  }
  createGrid();
}

std::string rtrim(std::string s) {
  std::string ret = "";
  for (size_t i = 0; i < s.length(); i++) {
    if (s[i] == ' ')
      break;
    ret += s[i];
  }
  return ret;
}

bool Grid::isValidWord(std::string enteredWord) {
  std::string word = rtrim(enteredWord);
  if (binarySearch(wordlist, word) != -1)
    return true;
  if (binarySearch(wordlist_commonWords, word) != -1)
    return true;
  if (binarySearch(wordlist_genAlphaSlang, word) != -1)
    return true;
  if (binarySearch(wordlist_allWords, word) != -1)
    return true;

  return false;
}

void Grid::enterWord(std::string enteredWord) {
  std::string word = enteredWord;
  for (int i = 0; i < wordlength - enteredWord.length(); i++) {
    word += ' ';
  }
  if (isValidWord(word)) {
    int8_t letterIndex = 0;
    updateTileTypes(currentGuesses, word);
    for (size_t i = 0; i < word.length(); i++) {
      tiles[i + currentGuesses * wordlength].let = word[letterIndex++];
    }
    currentGuesses++;
  } else {
    didWin = false;
  }
}

bool isInside(std::string str, char c) {
  for (int i = 0; i < str.length(); i++) {
    if (c == str[i]) {
      return true;
    }
  }
  return false;
}

void Grid::updateTileTypes(int guess, std::string& enteredWord) {
  didWin = true;
  for (int i = 0; i < wordlength; i++) {
    if (guess >= maxGuesses) {
      didWin = false;
      return;
    }
    Tile* t = &tiles[i + guess * wordlength];
    if (enteredWord[i] == ' ') {
      t->setType(TYPE_NONE);
      continue;
    }
    if (getWord()[i] == enteredWord[i]) {
      t->setType(TYPE_GREEN);
    } else if (isInside(word, enteredWord[i])) {
      t->setType(TYPE_YELLOW);
      didWin = false;
    } else {
      t->setType(TYPE_GREY);
      didWin = false;
    }
  }
}

std::ostream& operator<<(std::ostream& out, Grid& grid) {
  const int tileSize = 1;
  std::string printStr;

  for (uint8_t lineIndex = 0; lineIndex < tileSize * grid.maxGuesses; lineIndex++) {
    for (uint8_t charIndex = 0; charIndex < tileSize * grid.wordlength; charIndex++) {
      const Tile t = grid.tiles[std::floor(charIndex / tileSize) +
                                grid.wordlength * (std::floor(lineIndex / tileSize))];
      char c = t.let;

      if (c == ' ') {
        c = '~';
      }
      switch (t.type) {
        case TYPE_GREY:
          printStr += "\033[0m";
          break;
        case TYPE_NONE:
          printStr += "\x1B[38;5;8m";
          break;
        case TYPE_GREEN:
          printStr += "\033[1;32m";
          break;
        case TYPE_YELLOW:
          printStr += "\033[1;33m";
          break;
      };
      printStr += c;
      printStr += "\033[0m";
      if (charIndex % tileSize == (tileSize + 1) % 2) {
        printStr += ' ';
      }
    }
    printStr += '\n';
  }
  return out << printStr;
}
