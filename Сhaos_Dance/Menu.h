#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Game;

class Menu {
 public:
  Menu(sf::RenderWindow&, Game&);

  void handleEvent(const sf::Event& event, float deltaTime);
  void update(float deltaTime);
  void render(float deltaTime);
  void selectItem();

 private:
  void createGradient();
  void createLogo();
  void createButtons();

  sf::RenderWindow& main_window;
  Game& game;

  // Градиент
  sf::VertexArray gradient;
  sf::Color topColor = sf::Color(20, 20, 40);
  sf::Color bottomColor = sf::Color(100, 50, 150);

  // Логотип
  sf::Texture logo_texture;
  std::unique_ptr<sf::Sprite> logo_sprite;

  // Пункты меню
  std::vector<std::string> buttons = {"Play", "Exit"};
  std::vector<sf::Text> main_menu_items;
  sf::Font font;
  int selected_item = 0;
  unsigned int defultButtonSize = 100;
  unsigned int selectButtonSize = 120;

  // Размеры окна
  float width = 0.0f;
  float height = 0.0f;
};
