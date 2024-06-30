#include <termios.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include "grid/grid.h"
#include "input/input.h"
#include "keycodes/keycodes.h"
#include "wordlists/wordlist.h"

struct config {
  bool debug;
  bool compatibility;
};

static const config conf = {
    .debug = true,
    .compatibility = false,
};

static struct termios oldTerm, newTerm;

void initTermiosRaw(int echo) {
  tcgetattr(0, &oldTerm);
  newTerm = oldTerm;
  newTerm.c_lflag &= ~ICANON;
  newTerm.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &newTerm);
  // Hide cursor
  std::cout << "\e[?25l" << std::flush;
}

void resetTermios() {
  tcsetattr(0, TCSANOW, &oldTerm);
}

void cleanup() {
  std::cout << "\033[0m" << std::flush;
  std::cout << "\e[?25h" << std::flush;
  resetTermios();
}

int initWordlist(int argc, char* argv[], wordlistID& wordlist) {
  if (argc <= 1) {
    std::cout << "\nWORDLISTS\n" << std::endl;
    std::cout << "Classic:   Classic Wordle" << std::endl;
    std::cout << "Full:      Wordle Hard Mode" << std::endl;
    std::cout << "Alpha:     Wordle: Skibidi Rizz Edition" << std::endl;
    std::cout << "Insane:    Wordle, but it has every word." << std::endl;
  } else {
    auto typeArg = std::string(argv[1]);
    std::transform(typeArg.begin(), typeArg.end(), typeArg.begin(), ::tolower);
    if (typeArg == "classic") {
      wordlist = CLASSIC;
    } else if (typeArg == "full") {
      wordlist = FULL;
    } else if (typeArg == "alpha") {
      wordlist = ALPHA;
    } else if (typeArg == "insane") {
      wordlist = INSANE;
    } else {
      std::cout << "Wordlist invalid. Enter no args for a list." << std::endl;
      return 1;
    }
  }
  return 0;
}

void handleLoss(Grid g) {
  // The spaces here are necessary. Don't question it.
  std::cout << "\n            You Lose! The word was: " << g.getWord() << '\n';
}

void clearTerm() {
  std::cout << "\x1B[2J\x1B[H";
}

int main(int argc, char* argv[]) {
  wordlistID wordlist;
  int wordlistStatus = initWordlist(argc, argv, wordlist);
  if (wordlistStatus == 1)
    return 1;
  if (argc <= 1) {
    return 1;
  }

  Grid g = Grid(6, wordlist, argv[0]);
  Input in = Input(g);

  clearTerm();

  if (conf.compatibility) {
    if (conf.debug)
      std::cout << g.getWord() << std::endl;
    std::cout << g << std::endl;
    while (g.getCurrentGuesses() < g.maxGuesses) {
      std::string word;
      std::cout << "> " << std::flush;
      std::getline(std::cin, word);
      g.enterWord(word);
      std::cout << g << std::endl;
      if (g.didWin) {
        std::cout << "\nYou Win!" << std::endl;
        cleanup();
        return 0;
      }
    }
  } else {
    initTermiosRaw(0);

    std::cout << g << '\n' << in << std::endl;
    char c;
    while (g.getCurrentGuesses() < g.maxGuesses) {
      clearTerm();
      ssize_t bytesRead = read(STDIN_FILENO, &c, 1);
      if (isValidKey(int(c))) {
        in.addChar(c);
      } else if (int(c) == 10) {
        g.enterWord(in.inputChars);
        in.inputChars = "";
        if (g.didWin) {
          std::cout << g << "\nYou Win!" << std::endl;
          cleanup();
          return 0;
        }
      }
      std::cout << g << '\n' << in << std::endl;
    }
  }

  handleLoss(g);
  cleanup();
}
