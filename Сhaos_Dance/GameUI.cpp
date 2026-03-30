#include "GameUI.h"

GameUI::GameUI() : hearts(PlayerConfig::MAX_HP), level_text(font) {
  if (Loader::loadFontFromResources(IDR_TTF1, font)) {
    level_text.setFont(font);
  }
  level_text.setCharacterSize(58);
  level_text.setFillColor(sf::Color::Yellow);
  level_text.setString("Level: 1");
}
void GameUI::alignment(sf::RenderWindow& window) {
  sf::FloatRect bounds = level_text.getLocalBounds();
  level_text.setOrigin({bounds.size.x, 0});
  level_text.setPosition(
      {static_cast<float>(window.getSize().x) - 20.0f, 20.0f});
}

void GameUI::setHealth(int health) { hearts.setHealth(health); }

void GameUI::addToCombo() { combo.addToCombo(); }

void GameUI::resetCombo() { combo.resetCombo(); }

void GameUI::setLevel(int level) {
  current_level = level;
  level_text.setString("Level: " + std::to_string(level));
}

void GameUI::draw(sf::RenderWindow& window) {
  alignment(window);
  window.draw(level_text);
  hearts.draw(window);
  combo.draw(window);
}
