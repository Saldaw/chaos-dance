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
          tileColor =
              sf::Color(tileColor.r / 2, tileColor.g / 2, tileColor.b / 2);
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

  for (unsigned int y = 0; y < grid_height; y++) {
    for (unsigned int x = 0; x < grid_width; x++) {
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

  // Cбрасываем visible для всех клеток
  for (unsigned int y = 0; y < grid_height; ++y) {
    for (unsigned int x = 0; x < grid_width; ++x) {
      cells[y][x].setVisible(false);
    }
  }

  int px = player->getGridPosition().x;
  int py = player->getGridPosition().y;
  int range = player->getViewRange();

  // Функция для проверки видимости по линии (DDA)
  auto isLineOfSightClear = [&](int x0, int y0, int x1, int y1) -> bool {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (x != x1 || y != y1) {
      if (x != x0 || y != y0) {
        if (getTile(x, y) != Tile::TileType::kFloor) {
          return false;
        }
      }
      int e2 = 2 * err;
      if (e2 > -dy) {
        err -= dy;
        x += sx;
      }
      if (e2 < dx) {
        err += dx;
        y += sy;
      }
    }
    return true;
  };

  for (int dy = -range; dy <= range; ++dy) {
    for (int dx = -range; dx <= range; ++dx) {
      int nx = px + dx;
      int ny = py + dy;
      if (nx < 0 || nx >= static_cast<int>(grid_width) || ny < 0 ||
          ny >= static_cast<int>(grid_height)) {
        continue;
      }
      if (dx * dx + dy * dy > range * range) {
        continue;
      }
      if (isLineOfSightClear(px, py, nx, ny)) {
        cells[ny][nx].setVisible(true);
        cells[ny][nx].openCell();
      }
    }
  }
}

std::optional<sf::Vector2<int>> Grid::findBestMove(
    const sf::Vector2<int>& start, const sf::Vector2<int>& target,
    const std::vector<sf::Vector2<int>>& directions) {
  // Если стартовая позиция не проходима
  if (!isWalkable(start.x, start.y)) {
    return std::nullopt;
  }

  // Если целевая позиция не проходима, ищем ближайшую проходимую
  sf::Vector2<int> actualTarget = target;
  if (!isWalkable(target.x, target.y)) {
    int bestDist = INT_MAX;
    bool found = false;

    for (int dy = -5; dy <= 5; ++dy) {
      for (int dx = -5; dx <= 5; ++dx) {
        int nx = target.x + dx;
        int ny = target.y + dy;

        if (nx >= 0 && nx < static_cast<int>(grid_width) && ny >= 0 &&
            ny < static_cast<int>(grid_height) && isWalkable(nx, ny)) {
          int dist = std::abs(dx) + std::abs(dy);
          if (dist < bestDist) {
            bestDist = dist;
            actualTarget = {nx, ny};
            found = true;
          }
        }
      }
    }

    if (!found) {
      return std::nullopt;
    }
  }

  // BFS для поиска пути
  struct Node {
    int x, y;
    int dist;
    sf::Vector2<int> firstStep;
  };

  std::vector<std::vector<bool>> visited(grid_height,
                                         std::vector<bool>(grid_width, false));
  std::queue<Node> queue;

  queue.push({start.x, start.y, 0, {0, 0}});
  visited[start.y][start.x] = true;

  while (!queue.empty()) {
    Node current = queue.front();
    queue.pop();

    // Проверяем, достигли ли цели
    if (current.x == actualTarget.x && current.y == actualTarget.y) {
      return current.firstStep;
    }

    // Исследуем соседей
    for (const auto& dir : directions) {
      int nx = current.x + dir.x;
      int ny = current.y + dir.y;

      // Проверка границ
      if (nx < 0 || nx >= static_cast<int>(grid_width) || ny < 0 ||
          ny >= static_cast<int>(grid_height)) {
        continue;
      }

      // Проверка, что клетка не посещена и проходима
      if (!visited[ny][nx] && isWalkable(nx, ny)) {
        visited[ny][nx] = true;

        sf::Vector2<int> firstStep = current.firstStep;
        if (current.dist == 0) {
          firstStep = dir;
        }

        queue.push({nx, ny, current.dist + 1, firstStep});
      }
    }
  }

  return std::nullopt;
}

bool Grid::isWalkable(int x, int y) {
  if (x < 0 || x >= static_cast<int>(grid_width) || y < 0 ||
      y >= static_cast<int>(grid_height)) {
    return false;
  }

  return getCell(x, y)->getTileType() == Tile::TileType::kFloor;
}
