#pragma once
#include <vector>

#include "LoadFromMemory.h"
#include "resource.h"
class HeartUI {
 private:
  sf::Texture heartTexture;
  std::vector<sf::Sprite> hearts;
  int maxHearts;
  int currentHealth;

 public:
  HeartUI(int maxHearts);

  void setHealth(int health);
  void draw(sf::RenderWindow& window);

 private:
  void createHeartTexture();
  void updateHearts();
};
