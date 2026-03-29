#include "BombCake.h"

#include "Grid.h"
#include "Player.h"

BombCake::BombCake(sf::Vector2<int> position, std::shared_ptr<Grid> grid)
    : Enemy(position, grid) {
  sprite_texture.loadFromFile("assets/BombÑupcakeAnim.png");
  sprite.setTexture(sprite_texture);
  animator.addAnimation("jump", 256, 256, 0, 0, 2, 0.2f);
  animator.addAnimation("attack", 256, 256, 0, 256, 1, 0.2f);
}

void BombCake::update(float deltaTime) { animator.update(deltaTime); }
void BombCake::beat() {
  if (state == EnemyState::Attack) {
    grid->removeObject(shared_from_this());
    return;
  }

  std::shared_ptr<Player> player = grid->getPlayer();
  sf::Vector2i pl_pos = player->getGridPosition();
  int dist = (pl_pos.x - grid_position.x) * (pl_pos.x - grid_position.x) +
             (pl_pos.y - grid_position.y) * (pl_pos.y - grid_position.y);
  if (dist <= distExplosion * distExplosion) {
    state = EnemyState::Attack;
    animator.play("attack", false);
    player->getDamage(EnemyDamage);
    return;
  } else if (dist <= visibilityRangeEnemy * visibilityRangeEnemy) {
    moveToPlayer(pl_pos);
  } else {
    RandomMove();
  }
  if (steps_to_explosion <= 0) {
    animator.play("attack", false);
    state = EnemyState::Attack;
    if (pl_pos == grid_position) {
      player->getDamage(EnemyDamage);
    }
  } else {
    animator.play("jump", false);
  }
}
void BombCake::moveToPlayer(sf::Vector2i pl_pos) {
  sf::Vector2i pos_new = grid_position;
  if (pl_pos.x > grid_position.x) pos_new.x += 1;
  if (pl_pos.x < grid_position.x) pos_new.x -= 1;
  if (pl_pos.y > grid_position.y) pos_new.y += 1;
  if (pl_pos.y < grid_position.y) pos_new.y -= 1;
  grid->moveObject(shared_from_this(), pos_new.x, pos_new.y);
  steps_to_explosion -= 1;
}
void BombCake::RandomMove() {
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
