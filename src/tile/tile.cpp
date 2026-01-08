#include "tile.h"

Tile::Tile() {};
Tile::Tile(char let) : let(let) {};
tileType Tile::setType(tileType newType) {
    type = newType;
    return type;
}
