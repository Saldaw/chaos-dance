#include "GameUI.h"

GameUI::GameUI() : hearts(PlayerConfig::START_HP) {}

void GameUI::setHealth(int health) { hearts.setHealth(health); }

void GameUI::addToCombo() { combo.addToCombo(); }

void GameUI::resetCombo() { combo.resetCombo(); }

void GameUI::draw(sf::RenderWindow& window) {
  hearts.draw(window);
  combo.draw(window);
}
