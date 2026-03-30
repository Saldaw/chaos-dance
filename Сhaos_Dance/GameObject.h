#pragma once
#include <SFML/Graphics.hpp>

#include "Config.h"
class Grid;

class GameObject : public std::enable_shared_from_this<GameObject> {
  sf::Color original_color;
  float hit_timer;
  bool is_hit;
  float hit_duration;

 protected:
  sf::Vector2<int> grid_position;
  sf::Texture sprite_texture;
  sf::Sprite sprite = sf::Sprite(sprite_texture);
  std::shared_ptr<Grid> grid;

 public:
  GameObject(sf::Vector2<int> position, std::shared_ptr<Grid> grid);
  void setGridPosition(sf::Vector2<int> newPosition);
  void flip(bool);

  virtual void update(float deltaTime);
  virtual void getDamage(int damage);

  void setRedEffect(float duration = OtherConfig::HIT_EFFECT_DURATION,
                    float degree = OtherConfig::HIT_EFFECT_DEGREE);
  void updateHitEffect(float deltaTime);

  void draw(sf::RenderWindow& window, float cellSize, float offsetX,
            float offsetY);
  sf::Vector2<int> getGridPosition() { return grid_position; }
};
