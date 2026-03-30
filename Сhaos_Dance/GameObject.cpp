#include "GameObject.h"

#include "Grid.h"

GameObject::GameObject(sf::Vector2<int> position, std::shared_ptr<Grid> grid)
    : grid_position(position), grid(grid), original_color(sf::Color::White) {}

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

void GameObject::update(float deltaTime) { updateHitEffect(deltaTime); }

void GameObject::updateHitEffect(float deltaTime) {
  if (is_hit) {
    hit_timer -= deltaTime;
    if (hit_timer <= 0) {
      sprite.setColor(original_color);
      is_hit = false;
    }
  }
}

void GameObject::setRedEffect(float duration, float degree) {
  if (!is_hit) {
    original_color = sprite.getColor();
  }
  sf::Color color = sf::Color(static_cast<uint8_t>(original_color.r),
                              static_cast<uint8_t>(original_color.g / degree),
                              static_cast<uint8_t>(original_color.b / degree));
  sprite.setColor(color);
  is_hit = true;
  hit_timer = duration;
  hit_duration = duration;
}

void GameObject::getDamage(int damage) { setRedEffect(); }
