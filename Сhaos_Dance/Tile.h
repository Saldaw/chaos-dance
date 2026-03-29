#pragma once
#include <SFML/Graphics.hpp>

class Tile {
 public:
  enum class TileType { kEmpty, kWall, kFloor, kExit };
  static sf::Color getTileColor(TileType type, int x, int y);
};
