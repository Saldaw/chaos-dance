#pragma once

#include "Animator.h"
#include "Config.h"
#include "GameObject.h"

class Player : public GameObject {
 private:
  int view_range;
  int hp;

  Animator animator = Animator(sprite);

 public:
  Player(sf::Vector2<int> position, int view_range, int hp,
         std::shared_ptr<Grid> grid);
  void update(float deltaTime) override;
  void getDamage(int damage) override;
  void playAnimation(std::string name);
  int getViewRange() { return view_range; }
  int getHp() { return hp; }
  void addHp(int newHp);
};
