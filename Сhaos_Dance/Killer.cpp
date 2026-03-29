#include "Killer.h"

#include "Grid.h"
#include "Player.h"
Killer::Killer(sf::Vector2<int> position, std::shared_ptr<Grid> grid)
    : Enemy(position, grid) {
  sprite_texture.loadFromFile("assets/KillerRebbit.png");
  sprite.setTexture(sprite_texture);
  animator.addAnimation("jump", 256, 256, 0, 0, 2, 0.2f);
  animator.addAnimation("attack", 256, 256, 0, 256, 2, 0.2f);
  animator.addAnimation("swing", 256, 256, 0, 512, 1, 0.2f);
}

void Killer::update(float deltaTime) { animator.update(deltaTime); }
void Killer::beat() {
  std::shared_ptr<Player> player = grid->getPlayer();
  sf::Vector2i pl_pos = player->getGridPosition();
  int dist = (pl_pos.x - grid_position.x) * (pl_pos.x - grid_position.x) +
             (pl_pos.y - grid_position.y) * (pl_pos.y - grid_position.y);

  if (state == EnemyState::PreparingAttack) {
    state = EnemyState::Attack;
    animator.play("attack", false);
    if (dist <= 1) {
      player->getDamage(EnemyDamage);
    }
    return;
  }

  if (dist <= 1) {
    state = EnemyState::PreparingAttack;
    animator.play("swing", false);
    return;
  }
  if (dist <= visibilityRangeEnemy * visibilityRangeEnemy) {
    moveToPlayer(pl_pos);
  } else {
    RandomMove();
  }
  animator.play("jump", false);
}
void Killer::moveToPlayer(sf::Vector2i pl_pos) {
  sf::Vector2i pos_new = grid_position;
  if (pl_pos.x > grid_position.x)
    pos_new.x += 1;
  else if (pl_pos.x < grid_position.x)
    pos_new.x -= 1;
  else if (pl_pos.y > grid_position.y)
    pos_new.y += 1;
  else if (pl_pos.y < grid_position.y)
    pos_new.y -= 1;
  grid->moveObject(shared_from_this(), pos_new.x, pos_new.y);
  steps_to_explosion -= 1;
}
void Killer::RandomMove() {
  std::vector<sf::Vector2<int>> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
  std::shuffle(directions.begin(), directions.end(), rng);
  for (auto& dir : directions) {
    int newX = grid_position.x + dir.x;
    int newY = grid_position.y + dir.y;
    if (grid->getCell(newX, newY)->getTileType() == Tile::TileType::kFloor) {
      grid->moveObject(shared_from_this(), newX, newY);
      break;
    }
  }
}
