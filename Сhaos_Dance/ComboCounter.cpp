#include "ComboCounter.h"

#include <string>

ComboCounter::ComboCounter() : combo(0), text(font) {
  font.openFromFile("assets/front.ttf");

  text.setFont(font);
  text.setCharacterSize(58);
  text.setFillColor(sf::Color::Yellow);
  text.setOutlineThickness(2);
  text.setPosition({100.0f, 60.0f});
}

void ComboCounter::addToCombo(int amount) {
  combo += amount;
  updateText();
}

void ComboCounter::resetCombo() {
  combo = 0;
  updateText();
}

void ComboCounter::updateText() {
  if (combo > 0) {
    text.setString("x" + std::to_string(combo));
  } else {
    text.setString("");
  }
}

void ComboCounter::draw(sf::RenderWindow& window) {
  if (combo > 0) {
    window.draw(text);
  }
}
