#include "Cell.h"
Cell::Cell(Tile::TileType tile) : cell_type(tile) {}
void Cell::addObject(std::shared_ptr<GameObject> obj) {
  objects.push_back(obj);
}

void Cell::removeObject(std::shared_ptr<GameObject> obj) {
  auto it = std::find(objects.begin(), objects.end(), obj);
  if (it != objects.end()) {
    objects.erase(it);
  }
}
