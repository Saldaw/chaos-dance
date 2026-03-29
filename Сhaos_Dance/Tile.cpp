#include "Tile.h"
sf::Color Tile::getTileColor(TileType type, int x, int y) {
  switch (type) {
    case Tile::TileType::kEmpty:
      return sf::Color(0, 0, 0);
      break;

    case Tile::TileType::kWall:
      if ((x + y) % 2 == 0) return sf::Color(40, 40, 50);
      return sf::Color(35, 35, 45);
      break;
    case Tile::TileType::kExit:
      return sf::Color(100, 0, 0);
      break;

    case Tile::TileType::kFloor:
      int hash = x * 374761393 + y * 668265263;
      hash = (hash ^ (hash >> 13)) * 1274126177;

      float value =
          0.6f + (hash & 0x7FFFFFFF) / static_cast<float>(0x7FFFFFFF) * 0.4f;
      return sf::Color(110 * value, 129 * value, 67 * value);
      break;
  }
}
