#pragma once
#include <random>

#include "Config.h"
#include "Enemy.h"
class Killer : public Enemy {
 private:
  std::mt19937 rng;
  void moveToPlayer(sf::Vector2i pl_pos);
  void RandomMove();

 public:
  Killer(sf::Vector2<int> position, std::shared_ptr<Grid> grid);
  void beat() override;
  void update(float deltaTime) override;
};
