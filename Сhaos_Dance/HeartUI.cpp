#include "HeartUI.h"

HeartUI::HeartUI(int maxHearts)
    : maxHearts(maxHearts), currentHealth(maxHearts) {
  createHeartTexture();

  for (int i = 0; i < maxHearts; i++) {
    sf::Sprite heart(heartTexture);
    heart.setPosition({10.0f + i * 60.0f, 10.0f});
    heart.setScale({0.2f, 0.2f});
    hearts.push_back(heart);
  }
}

void HeartUI::createHeartTexture() {
  Loader::loadTextureFromResources(IDB_PNG2, heartTexture);
}

void HeartUI::setHealth(int health) {
  currentHealth = std::clamp(health, 0, maxHearts);
  updateHearts();
}

void HeartUI::updateHearts() {
  hearts.clear();

  for (int i = 0; i < currentHealth; i++) {
    sf::Sprite heart(heartTexture);
    heart.setPosition({10.0f + i * 60.0f, 10.0f});
    heart.setScale({0.2f, 0.2f});
    hearts.push_back(heart);
  }
}

void HeartUI::draw(sf::RenderWindow& window) {
  for (auto& heart : hearts) {
    window.draw(heart);
  }
}
