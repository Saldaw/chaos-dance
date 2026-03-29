#include "Chest.h"

#include "Grid.h"

Chest::Chest(sf::Vector2<int> position, std::shared_ptr<Grid> grid)
    : GameObject(position, grid) {
  sprite_texture.loadFromFile("assets/Cheast.png");
  sprite.setTexture(sprite_texture);
  sf::IntRect rect;
  rect.position.x = 0;
  rect.position.y = 0;
  rect.size.x = sprite_texture.getSize().x;
  rect.size.y = sprite_texture.getSize().y;
  sprite.setTextureRect(rect);
}

void Chest::update(float deltaTime) {}
void Chest::getDamage(int damage) {
  grid->getPlayer()->addHp(1);
  grid->removeObject(shared_from_this());
}
