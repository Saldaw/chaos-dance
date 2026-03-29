#pragma once
#include "Animator.h"
#include "GameObject.h"

enum class EnemyState { Idle, Move, PreparingAttack, Attack };

class Enemy : public GameObject {
 protected:
  int hp = EnemyHP;
  EnemyState state = EnemyState::Idle;
  Animator animator = Animator(sprite);

 public:
  Enemy(sf::Vector2<int> position, std::shared_ptr<Grid> grid);
  virtual void beat() = 0;
  void getDamage(int damage) override;
};
