#pragma once

#include "Config.h"
#include "GameUI.h"
#include "Grid.h"
#include "LoadFromMemory.h"
#include "MapGenerator.h"
#include "RhythmEngine.h"
#include "resource.h"

class Menu;

class Game {
 public:
  ~Game();
  enum class GameState { Menu, Playing };
  void setState(GameState);
  Game();
  void startGame();
  void gameOver();
  void gameWin();

 private:
  sf::Font font;
  sf::Text message_text;
  int hits_to_reset_message = 1;
  std::unique_ptr<MusicResource> music_resource;
  GameState current_state;
  sf::RenderWindow main_window;
  std::unique_ptr<Menu> main_menu;
  std::shared_ptr<Player> player;
  std::shared_ptr<Grid> grid;
  std::unique_ptr<GameUI> ui;
  std::unique_ptr<RhythmEngine> rhythm;
  std::unique_ptr<MapGenerator> map_gen;

  int level = 1;

  void genMap();
  void handleGameplayEvent(const sf::Event& event, float deltaTime);
  void attemptPlayerMove(const sf::Vector2i& direction);
  void restartGame(bool saveState);
  void centerText(sf::Text& text);

  bool canMoveHere(sf::Vector2i pos);
  void handleEvents(float);
  void update(float);
  void render(float);
};
