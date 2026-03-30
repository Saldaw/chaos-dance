#include "Enemy.h"

#include "Grid.h"

Enemy::Enemy(sf::Vector2<int> position, std::shared_ptr<Grid> grid, int HP)
    : GameObject(position, grid), hp(HP) {}

void Enemy::getDamage(int damage) {
  hp -= damage;
  if (hp <= 0) {
    grid->removeObject(shared_from_this());
  }
  GameObject::getDamage(damage);
}
