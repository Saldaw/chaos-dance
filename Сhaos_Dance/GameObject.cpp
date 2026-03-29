#include "GameObject.h"

#include "Grid.h"

GameObject::GameObject(sf::Vector2<int> position, std::shared_ptr<Grid> grid)
    : grid_position(position), grid(grid) {}

void GameObject::setGridPosition(sf::Vector2<int> newPosition) {
  grid_position = newPosition;
}
void GameObject::flip(bool f) {
  sf::Vector2f start_scale = sprite.getScale();
  if ((start_scale.x > 0 && !f) || (start_scale.x < 0 && f)) {
    sf::Vector2f fliped_scale = {start_scale.x * -1, start_scale.y};
    sprite.setScale(fliped_scale);
  }
}

void GameObject::draw(sf::RenderWindow& window, float cellSize, float offsetX,
                      float offsetY) {
  float screenX = offsetX + grid_position.x * cellSize + cellSize / 2.0f;
  float screenY = offsetY + grid_position.y * cellSize + cellSize / 2.0f;

  sf::FloatRect bounds = sprite.getLocalBounds();
  float scale =
      cellSize / std::max(std::abs(bounds.size.x), std::abs(bounds.size.y));
  sprite.setScale(
      {scale * ((sprite.getScale().x > 0) - (sprite.getScale().x < 0)), scale});

  // Центрируем и позиционируем
  sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
  sprite.setPosition({screenX, screenY});

  window.draw(sprite);
}
