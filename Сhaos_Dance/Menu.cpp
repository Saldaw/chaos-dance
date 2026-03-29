#include "Menu.h"

#include "Game.h"

void Menu::createGradient() {
  gradient.setPrimitiveType(sf::PrimitiveType::Triangles);
  gradient.resize(6);

  gradient[0] = sf::Vertex({{0.0f, 0.0f}, topColor});
  gradient[1] = sf::Vertex({{width, 0.0f}, topColor});
  gradient[2] = sf::Vertex({{0.0f, height}, bottomColor});
  gradient[3] = sf::Vertex({{width, 0.0f}, topColor});
  gradient[4] = sf::Vertex({{width, height}, bottomColor});
  gradient[5] = sf::Vertex({{0.0f, height}, bottomColor});
}

void Menu::createLogo() {
  logo_sprite = std::make_unique<sf::Sprite>(logo_texture);
  sf::FloatRect logoBounds = logo_sprite->getLocalBounds();
  logo_sprite->setOrigin({logoBounds.size.x / 2.0f, 0.0f});
  logo_sprite->setPosition({width / 2.0f, height * 0.1f});
}

void Menu::createButtons() {
  for (size_t i = 0; i < buttons.size(); ++i) {
    sf::Text text(font);
    text.setString(buttons[i]);
    text.setCharacterSize(defultButtonSize);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                    textBounds.position.y + textBounds.size.y / 2.0f});
    text.setPosition({width / 2.0f, height * 0.6f + i * selectButtonSize});

    main_menu_items.push_back(text);
  }
}

Menu::Menu(sf::RenderWindow& window, Game& game_)
    : main_window(window), game(game_) {
  sf::Vector2u windowSize = main_window.getSize();
  width = static_cast<float>(windowSize.x);
  height = static_cast<float>(windowSize.y);

  if (!font.openFromFile("assets/front.ttf") ||
      !logo_texture.loadFromFile("assets/logo.png")) {
    main_window.close();
  }

  createGradient();
  createButtons();
  createLogo();
}
void Menu::handleEvent(const sf::Event& event, float deltaTime) {
  if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->scancode == sf::Keyboard::Scancode::Up ||
        keyPressed->scancode == sf::Keyboard::Scancode::W) {
      if (selected_item > 0) {
        selected_item--;
      }
    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Down ||
               keyPressed->scancode == sf::Keyboard::Scancode::S) {
      if (selected_item < static_cast<int>(main_menu_items.size()) - 1) {
        selected_item++;
      }
    } else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
      selectItem();
    }
  }
}

void Menu::update(float deltaTime) {
  for (size_t i = 0; i < main_menu_items.size(); ++i) {
    if (i == selected_item) {
      main_menu_items[i].setFillColor(sf::Color::Red);
      main_menu_items[i].setCharacterSize(selectButtonSize);
    } else {
      main_menu_items[i].setFillColor(sf::Color::White);
      main_menu_items[i].setCharacterSize(defultButtonSize);
    }
  }
}

void Menu::render(float deltaTime) {
  main_window.draw(gradient);
  main_window.draw(*logo_sprite);

  // Рисуем пункты меню
  for (auto& item : main_menu_items) {
    main_window.draw(item);
  }
}

void Menu::selectItem() {
  switch (selected_item) {
    case 0:  // Play
      game.setState(Game::GameState::Playing);
      break;
    case 1:  // Exit
      main_window.close();
      break;
  }
}
