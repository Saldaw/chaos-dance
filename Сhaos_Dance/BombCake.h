#pragma once
#include <random>

#include "Config.h"
#include "Enemy.h"

class BombCake : public Enemy {
 private:
  std::mt19937 rng;
  int steps_to_explosion = EnemyConfig::STEPS_TO_EXPLOSION;
  void moveToPlayer(sf::Vector2i pl_pos);
  void RandomMove();

 public:
  BombCake(sf::Vector2<int> position, std::shared_ptr<Grid> grid);
  void beat() override;
  void update(float deltaTime) override;
};
