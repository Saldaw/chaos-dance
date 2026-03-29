#include "Grid.h"

Grid::Grid(unsigned int width, unsigned int height,
           std::shared_ptr<Player> player)
    : grid_width(width),
      grid_height(height),
      player(player),
      rng(std::random_device{}()) {
  cells.resize(height, std::vector<Cell>(width));
  cell_shape.setFillColor(sf::Color::White);
}
void Grid::updateSize(sf::Vector2u windowSize) {
  window_width = static_cast<int>(windowSize.x);
  window_height = static_cast<int>(windowSize.y);
  recalcCellSize();
  recalcOffsets();
}

Tile::TileType Grid::getTile(int x, int y) {
  return getCell(x, y)->getTileType();
}
void Grid::setTile(int x, int y, Tile::TileType type) {
  getCell(x, y)->setTileType(type);
}
void Grid::addObject(std::shared_ptr<GameObject> obj, int x, int y) {
  cells[y][x].addObject(obj);
  objects.push_back(obj);
}
void Grid::removeObject(std::shared_ptr<GameObject> obj) {
  auto it = std::find(objects.begin(), objects.end(), obj);
  if (it != objects.end()) {
    sf::Vector2i pos = obj->getGridPosition();
    getCell(pos.x, pos.y)->removeObject(obj);
    objects.erase(it);
  }
}
void Grid::recalcOffsets() {
  float totalWidth = cell_size * grid_width;
  float totalHeight = cell_size * grid_height;

  offset_x = (window_width - totalWidth) / 2.0f;
  offset_y = (window_height - totalHeight) / 2.0f;
}
void Grid::draw(sf::RenderWindow& window) {
  if (cell_size <= 0.0f) return;

  cell_shape.setSize({cell_size, cell_size});

  for (unsigned int y = 0; y < grid_height; ++y) {
    for (unsigned int x = 0; x < grid_width; ++x) {
      Cell& cell = cells[y][x];

      if (!cell.isOpened()) {
        cell_shape.setFillColor(sf::Color::Black);
      } else {
        sf::Color tileColor = Tile::getTileColor(cell.getTileType(), x, y);
        if (!cell.isVisible()) {
          tileColor = sf::Color(tileColor.r * 0.5f, tileColor.g * 0.5f,
                                tileColor.b * 0.5f);
        }
        cell_shape.setFillColor(tileColor);
      }
      cell_shape.setPosition(
          {offset_x + x * cell_size, offset_y + y * cell_size});
      window.draw(cell_shape);

      if (cell.isVisible()) {
        for (std::shared_ptr<GameObject> obj : cell.getObjects()) {
          obj->draw(window, cell_size, offset_x, offset_y);
        }
      }
    }
  }
}

std::optional<std::pair<int, int>> Grid::findRandomFloorTile() {
  if (grid_height == 0 || grid_width == 0) {
    return std::nullopt;
  }

  std::vector<std::pair<int, int>> floorTiles;

  for (int y = 0; y < grid_height; y++) {
    for (int x = 0; x < grid_width; x++) {
      if (cells[y][x].getTileType() == Tile::TileType::kFloor &&
          cells[y][x].getObjects().empty()) {
        floorTiles.push_back({x, y});
      }
    }
  }

  if (floorTiles.empty()) {
    return std::nullopt;
  }

  std::uniform_int_distribution<int> dist(
      0, static_cast<int>(floorTiles.size()) - 1);
  return floorTiles[dist(rng)];
}
Cell* Grid::getCell(unsigned int x, unsigned int y) {
  if (x < 0 || x >= grid_width || y < 0 || y >= grid_height) return nullptr;
  return &cells[y][x];
}
void Grid::moveObject(std::shared_ptr<GameObject> obj, int newX, int newY) {
  // Удаляем из старой клетки, добавляем в новую
  Cell* oldCell = getCell(obj->getGridPosition().x, obj->getGridPosition().y);
  oldCell->removeObject(obj);

  obj->setGridPosition({newX, newY});
  Cell* newCell = getCell(newX, newY);
  newCell->addObject(obj);
}

void Grid::spawnСhest(int count) {
  for (int i = 0; i < count; i++) {
    auto cords = findRandomFloorTile();
    if (cords) {
      auto pos = sf::Vector2<int>(cords.value().first, cords.value().second);
      auto chest = new Chest(pos, shared_from_this());
      addObject(std::shared_ptr<GameObject>(chest), pos.x, pos.y);
    }
  }
}
void Grid::spawnBombs(int count) {
  for (int i = 0; i < count; i++) {
    auto cords = findRandomFloorTile();
    if (cords) {
      auto pos = sf::Vector2<int>(cords.value().first, cords.value().second);
      auto bomb_cake = new BombCake(pos, shared_from_this());
      addObject(std::shared_ptr<GameObject>(bomb_cake), pos.x, pos.y);
    }
  }
}
void Grid::spawnKillers(int count) {
  for (int i = 0; i < count; i++) {
    auto cords = findRandomFloorTile();
    if (cords) {
      auto pos = sf::Vector2<int>(cords.value().first, cords.value().second);
      auto killer = new Killer(pos, shared_from_this());
      addObject(std::shared_ptr<GameObject>(killer), pos.x, pos.y);
    }
  }
}
void Grid::beat() {
  for (auto& obj : objects) {
    if (Enemy* enemy = dynamic_cast<Enemy*>(obj.get())) {
      enemy->beat();
    }
  }
}
void Grid::update(float deltaTime) {
  for (auto& obj : objects) {
    obj->update(deltaTime);
  }
}
std::vector<std::shared_ptr<GameObject>> Grid::getObjectsAt(int x, int y) {
  Cell* cell = getCell(x, y);
  if (cell) return cell->getObjects();
  return {};
}

void Grid::recalcCellSize() {
  if (window_width == 0 || window_height == 0) return;

  float maxWidth = window_width * 0.8f;
  float maxHeight = window_height * 0.8f;

  float cellByWidth = maxWidth / static_cast<float>(grid_width);
  float cellByHeight = maxHeight / static_cast<float>(grid_height);

  cell_size = std::min(cellByWidth, cellByHeight);
}

void Grid::updateFogOfWar() {
  if (!player) return;

  // Сначала сбрасываем visible для всех клеток
  for (unsigned int y = 0; y < grid_height; ++y) {
    for (unsigned int x = 0; x < grid_width; ++x) {
      cells[y][x].setVisible(false);
    }
  }

  int px = player->getGridPosition().x;
  int py = player->getGridPosition().y;
  int range = player->getViewRange();

  for (int dy = -range; dy <= range; ++dy) {
    for (int dx = -range; dx <= range; ++dx) {
      int nx = px + dx;
      int ny = py + dy;
      if (nx < 0 || nx >= static_cast<int>(grid_width) || ny < 0 ||
          ny >= static_cast<int>(grid_height))
        continue;

      if (dx * dx + dy * dy <= range * range) {
        cells[ny][nx].setVisible(true);
        cells[ny][nx].openCell();
      }
    }
  }
}
