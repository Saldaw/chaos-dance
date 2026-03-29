#include "MapGenerator.h"

#include <algorithm>
#include <iostream>
#include <queue>

MapGenerator::MapGenerator(int w, int h)
    : width(w), height(h), rng(std::random_device{}()) {
  map.resize(height,
             std::vector<Tile::TileType>(width, Tile::TileType::kEmpty));
}

void MapGenerator::initializeMap() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      map[y][x] = Tile::TileType::kFloor;  // Вся карта - пол
    }
  }
}

bool MapGenerator::isInBounds(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

void MapGenerator::addOuterWalls() {
  // Верхняя и нижняя стены
  for (int x = 0; x < width; x++) {
    if (isInBounds(x, 0)) map[0][x] = Tile::TileType::kWall;
    if (isInBounds(x, height - 1)) map[height - 1][x] = Tile::TileType::kWall;
  }

  // Левая и правая стены
  for (int y = 0; y < height; y++) {
    if (isInBounds(0, y)) map[y][0] = Tile::TileType::kWall;
    if (isInBounds(width - 1, y)) map[y][width - 1] = Tile::TileType::kWall;
  }
}

bool MapGenerator::canPlaceWall(int x, int y, bool horizontal, int length) {
  for (int i = 0; i < length; i++) {
    int cx = horizontal ? x + i : x;
    int cy = horizontal ? y : y + i;

    // Проверка границ
    if (!isInBounds(cx, cy)) return false;

    // Не ставим стены поверх других стен
    if (map[cy][cx] == Tile::TileType::kWall) return false;

    // Проверка, что стена не заблокирует проход (оставляем проходы)
    if (i > 0 && i < length - 1) {
      // Проверяем соседние клетки
      if (horizontal) {
        if (isInBounds(cx, cy - 1) && map[cy - 1][cx] == Tile::TileType::kWall)
          return false;
        if (isInBounds(cx, cy + 1) && map[cy + 1][cx] == Tile::TileType::kWall)
          return false;
      } else {
        if (isInBounds(cx - 1, cy) && map[cy][cx - 1] == Tile::TileType::kWall)
          return false;
        if (isInBounds(cx + 1, cy) && map[cy][cx + 1] == Tile::TileType::kWall)
          return false;
      }
    }
  }
  return true;
}

void MapGenerator::placeWall(int x, int y, bool horizontal, int length) {
  for (int i = 0; i < length; i++) {
    int cx = horizontal ? x + i : x;
    int cy = horizontal ? y : y + i;
    if (isInBounds(cx, cy)) {
      map[cy][cx] = Tile::TileType::kWall;
    }
  }
}

void MapGenerator::addInnerWalls() {
  std::uniform_int_distribution<int> xDist(2, width - 3);
  std::uniform_int_distribution<int> yDist(2, height - 3);
  std::uniform_int_distribution<int> lengthDist(MIN_WALL_LENGTH,
                                                MAX_WALL_LENGTH);
  std::uniform_int_distribution<int> boolDist(0, 1);
  std::uniform_real_distribution<float> densityDist(0.0f, 1.0f);

  int totalCells = (width - 4) * (height - 4);
  int maxWalls = static_cast<int>(totalCells * WALL_DENSITY);
  int wallsPlaced = 0;
  int attempts = 0;
  const int MAX_ATTEMPTS = 500;

  while (wallsPlaced < maxWalls && attempts < MAX_ATTEMPTS) {
    attempts++;

    int x = xDist(rng);
    int y = yDist(rng);
    bool horizontal = boolDist(rng) == 1;
    int length = lengthDist(rng);

    // Не ставим стены слишком близко к краям
    if (horizontal && x + length >= width - 2) continue;
    if (!horizontal && y + length >= height - 2) continue;

    if (canPlaceWall(x, y, horizontal, length)) {
      placeWall(x, y, horizontal, length);
      wallsPlaced++;
    }
  }
}

void MapGenerator::floodFill(int startX, int startY,
                             std::vector<std::vector<bool>>& visited) {
  if (!isInBounds(startX, startY) || visited[startY][startX] ||
      map[startY][startX] != Tile::TileType::kFloor) {
    return;
  }

  std::queue<std::pair<int, int>> queue;
  queue.push({startX, startY});
  visited[startY][startX] = true;

  int dx[] = {0, 1, 0, -1};
  int dy[] = {-1, 0, 1, 0};

  while (!queue.empty()) {
    auto [x, y] = queue.front();
    queue.pop();

    for (int i = 0; i < 4; i++) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (isInBounds(nx, ny) && !visited[ny][nx] &&
          map[ny][nx] == Tile::TileType::kFloor) {
        visited[ny][nx] = true;
        queue.push({nx, ny});
      }
    }
  }
}

bool MapGenerator::isFloorConnected() {
  std::vector<std::vector<bool>> visited(height,
                                         std::vector<bool>(width, false));

  // Находим первую клетку пола
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (map[y][x] == Tile::TileType::kFloor) {
        floodFill(x, y, visited);

        // Проверяем, все ли клетки пола посещены
        for (int y2 = 0; y2 < height; y2++) {
          for (int x2 = 0; x2 < width; x2++) {
            if (map[y2][x2] == Tile::TileType::kFloor && !visited[y2][x2]) {
              return false;
            }
          }
        }
        return true;
      }
    }
  }
  return false;  // Нет клеток пола
}

void MapGenerator::ensureConnectivity() {
  const int MAX_ITERATIONS = 10;

  for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
    if (isFloorConnected()) {
      return;  // Все хорошо
    }

    // Удаляем некоторые стены для восстановления связности
    std::vector<std::pair<int, int>> walls;
    for (int y = 1; y < height - 1; y++) {
      for (int x = 1; x < width - 1; x++) {
        if (map[y][x] == Tile::TileType::kWall) {
          walls.push_back({x, y});
        }
      }
    }

    // Перемешиваем и удаляем несколько стен
    std::shuffle(walls.begin(), walls.end(), rng);
    int wallsToRemove = std::min(5, static_cast<int>(walls.size()));

    for (int i = 0; i < wallsToRemove; i++) {
      auto [x, y] = walls[i];
      map[y][x] = Tile::TileType::kFloor;
    }
  }
}

void MapGenerator::generate() {
  initializeMap();
  addOuterWalls();
  addInnerWalls();
  ensureConnectivity();
  map[height - 2][width - 2] = Tile::TileType::kExit;
}
