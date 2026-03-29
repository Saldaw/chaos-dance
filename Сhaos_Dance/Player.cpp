#include "Player.h"

#include "Game.h"
#include "Grid.h"
#include "LoadFromMemory.h"
#include "resource.h"

Player::Player(sf::Vector2<int> position, int view_range,
               std::shared_ptr<Grid> grid)
    : view_range(view_range), GameObject(position, grid) {
  Loader::loadTextureFromResources(IDB_PNG3, sprite_texture);
  sprite.setTexture(sprite_texture);
  animator.addAnimation("attack", 256, 256, 0, 0, 2, 0.2f);
  animator.addAnimation("jump", 256, 256, 0, 256, 2, 0.2f);
  animator.addAnimation("idle", 256, 256, 0, 512, 2, 0.2f);
}
void Player::update(float deltaTime) { animator.update(deltaTime); }
void Player::playAnimation(std::string name) { animator.play(name, false); }
void Player::getDamage(int damage) {
  if (damage >= hp) {
    hp = 0;
  } else {
    hp -= damage;
  }
}
void Player::addHp(int newHP) { hp = std::min(hp + newHP, maxHP); }
