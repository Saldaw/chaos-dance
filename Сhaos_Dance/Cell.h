#pragma once

#include "GameObject.h"
#include "Tile.h"

class Cell {
 private:
  Tile::TileType cell_type;
  bool opened = false;
  bool visible = false;
  std::vector<std::shared_ptr<GameObject>> objects;

 public:
  Cell(Tile::TileType tile = Tile::TileType::kFloor);

  void addObject(std::shared_ptr<GameObject> obj);
  void removeObject(std::shared_ptr<GameObject> obj);

  // Сеттеры
  void setTileType(Tile::TileType type) { cell_type = type; }
  void openCell() { opened = true; }
  void setVisible(bool newVisible) { visible = newVisible; }
  // Геттеры
  Tile::TileType getTileType() { return cell_type; }
  bool isOpened() { return opened; }
  bool isVisible() { return visible; }
  std::vector<std::shared_ptr<GameObject>>& getObjects() { return objects; }
};
