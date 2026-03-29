#include "GameUI.h"

#include "Options.h"

GameUI::GameUI() : hearts(maxHP) {}

void GameUI::setHealth(int health) { hearts.setHealth(health); }

void GameUI::addToCombo() { combo.addToCombo(); }

void GameUI::resetCombo() { combo.resetCombo(); }

void GameUI::draw(sf::RenderWindow& window) {
  hearts.draw(window);
  combo.draw(window);
}
