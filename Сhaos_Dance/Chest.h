#pragma once

#include "GameObject.h"

class Chest : public GameObject {
 public:
  Chest(sf::Vector2<int> position, std::shared_ptr<Grid> grid);
  void update(float deltaTime) override;
  void getDamage(int damage) override;
};
