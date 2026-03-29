#pragma once

#include "GameUI.h"
#include "Grid.h"
#include "MapGenerator.h"
#include "RhythmEngine.h"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

class Menu;

class Game {
 public:
  ~Game();
  enum class GameState { Menu, Playing };
  void setState(GameState);
  Game();
  void startGame();
  void openMenu();
  void gameOver();
  void gameWin();

 private:
  sf::Font font;
  sf::Text result_text;
  bool is_active = true;
  sf::Music music;
  GameState current_state;
  sf::RenderWindow main_window;
  std::unique_ptr<Menu> main_menu;
  std::shared_ptr<Player> player;
  std::shared_ptr<Grid> grid;
  std::unique_ptr<GameUI> ui;
  std::unique_ptr<RhythmEngine> rhythm;
  std::unique_ptr<MapGenerator> map_gen;

  void handleGameplayEvent(const sf::Event& event, float deltaTime);
  void attemptPlayerMove(const sf::Vector2i& direction);

  bool canMoveHere(sf::Vector2i pos);
  void handleEvents(float);
  void update(float);
  void render(float);
};
