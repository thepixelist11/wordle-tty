#pragma once

#include <unordered_set>
enum Keys {
    KEY_A     = 97,
    KEY_B     = 98,
    KEY_C     = 99,
    KEY_D     = 100,
    KEY_E     = 101,
    KEY_F     = 102,
    KEY_G     = 103,
    KEY_H     = 104,
    KEY_I     = 105,
    KEY_J     = 106,
    KEY_K     = 107,
    KEY_L     = 108,
    KEY_M     = 109,
    KEY_N     = 110,
    KEY_O     = 111,
    KEY_P     = 112,
    KEY_Q     = 113,
    KEY_R     = 114,
    KEY_S     = 115,
    KEY_T     = 116,
    KEY_U     = 117,
    KEY_V     = 118,
    KEY_W     = 119,
    KEY_X     = 120,
    KEY_Y     = 121,
    KEY_Z     = 122,
    KEY_SPACE = 32,
    KEY_BS    = 127,
};

static bool isValidKey(int key) {
    static const std::unordered_set<int> validKeys = {
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G,     KEY_H,  KEY_I, KEY_J,
        KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q,     KEY_R,  KEY_S, KEY_T,
        KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, KEY_SPACE, KEY_BS,

    };
    return validKeys.find(key) != validKeys.end();
}
