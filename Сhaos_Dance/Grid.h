#pragma once

#include <optional>
#include <queue>
#include <random>

#include "BombCake.h"
#include "Cell.h"
#include "Chest.h"
#include "Enemy.h"
#include "Killer.h"
#include "Player.h"
#include "Tile.h"

class Grid : public std::enable_shared_from_this<Grid> {
 private:
  // Параметры отрисовки
  unsigned int grid_width, grid_height;
  unsigned int window_width, window_height;
  float cell_size;
  float offset_x, offset_y;
  std::mt19937 rng;

  sf::RectangleShape cell_shape;
  std::shared_ptr<Player> player;
  std::vector<std::vector<Cell>> cells;
  std::vector<std::shared_ptr<GameObject>> objects;

  void recalcCellSize();
  void recalcOffsets();

  std::optional<std::pair<int, int>> findRandomFloorTile();

 public:
  Grid(unsigned int width, unsigned int height, std::shared_ptr<Player> player);
  Grid() = default;
  void updateSize(sf::Vector2u windowSize);
  void setTile(int x, int y, Tile::TileType type);
  Tile::TileType getTile(int x, int y);
  void draw(sf::RenderWindow& window);
  void updateFogOfWar();
  Cell* getCell(unsigned int x, unsigned int y);
  void addObject(std::shared_ptr<GameObject> obj, int x, int y);
  void removeObject(std::shared_ptr<GameObject> obj);
  void moveObject(std::shared_ptr<GameObject> obj, int newX, int newY);
  void spawnСhest(int count);
  void spawnBombs(int count);
  void spawnKillers(int count);
  void beat();
  void update(float deltaTime);

  std::optional<sf::Vector2<int>> findBestMove(
      const sf::Vector2<int>& start, const sf::Vector2<int>& target,
      const std::vector<sf::Vector2<int>>& directions);

  bool isWalkable(int x, int y);
  std::vector<std::shared_ptr<GameObject>> getObjectsAt(int x, int y);
  int getGridWidth() const { return grid_width; }
  int getGridHeight() const { return grid_height; }
  float getCellSize() const { return cell_size; }
  std::shared_ptr<Player> getPlayer() const { return player; }
};
