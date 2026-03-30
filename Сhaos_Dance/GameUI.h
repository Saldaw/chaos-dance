#pragma once
#include <SFML/Graphics.hpp>

#include "ComboCounter.h"
#include "Config.h"
#include "HeartUI.h"

class GameUI {
 private:
  HeartUI hearts;
  ComboCounter combo;

 public:
  GameUI();

  void setHealth(int health);
  void addToCombo();
  void resetCombo();
  void update(float deltaTime);
  void draw(sf::RenderWindow& window);
  int getCombo() { return combo.getCombo(); }
};
