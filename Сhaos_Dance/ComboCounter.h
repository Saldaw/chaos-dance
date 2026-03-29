#pragma once
#include <SFML/Graphics.hpp>

#include "LoadFromMemory.h"
#include "resource.h"

class ComboCounter {
 private:
  sf::Font font;
  sf::Text text;
  int combo;

 public:
  ComboCounter();

  void addToCombo(int amount = 1);
  void resetCombo();
  void draw(sf::RenderWindow& window);
  int getCombo() { return combo; }

 private:
  void updateText();
};
