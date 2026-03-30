#pragma once
#include <random>
#include <vector>

#include "Config.h"
#include "Tile.h"

class MapGenerator {
 private:
  int width;
  int height;
  std::vector<std::vector<Tile::TileType>> map;
  std::mt19937 rng;

 public:
  MapGenerator(int w, int h);

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
