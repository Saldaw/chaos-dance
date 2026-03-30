#pragma once
#include <SFML/Graphics.hpp>

class Grid;

class GameObject : public std::enable_shared_from_this<GameObject> {
 protected:
  sf::Vector2<int> grid_position;
  sf::Texture sprite_texture;
  sf::Sprite sprite = sf::Sprite(sprite_texture);
  std::shared_ptr<Grid> grid;

 public:
  GameObject(sf::Vector2<int> position, std::shared_ptr<Grid> grid);
  void setGridPosition(sf::Vector2<int> newPosition);
  void flip(bool);

  virtual void update(float deltaTime) = 0;
  virtual void getDamage(int damage) = 0;
  void draw(sf::RenderWindow& window, float cellSize, float offsetX,
            float offsetY);
  sf::Vector2<int> getGridPosition() { return grid_position; }
};
