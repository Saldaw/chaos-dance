#pragma once
#include <random>
#include <vector>

#include "Tile.h"

class MapGenerator {
 private:
  // Константы
  static constexpr int DEFAULT_WIDTH = 10;
  static constexpr int DEFAULT_HEIGHT = 15;
  static constexpr int WALL_THICKNESS = 1;
  static constexpr float WALL_DENSITY = 0.25f;  // 25% стен внутри
  static constexpr int MIN_WALL_LENGTH = 3;
  static constexpr int MAX_WALL_LENGTH = 8;

  int width;
  int height;
  std::vector<std::vector<Tile::TileType>> map;
  std::mt19937 rng;

 public:
  MapGenerator(int w = DEFAULT_WIDTH, int h = DEFAULT_HEIGHT);

  void generate();
  const std::vector<std::vector<Tile::TileType>>& getMap() const { return map; }

 private:
  void initializeMap();
  void addOuterWalls();
  void addInnerWalls();
  bool isInBounds(int x, int y) const;
  bool canPlaceWall(int x, int y, bool horizontal, int length);
  void placeWall(int x, int y, bool horizontal, int length);
  void ensureConnectivity();
  void floodFill(int startX, int startY,
                 std::vector<std::vector<bool>>& visited);
  bool isFloorConnected();
};
