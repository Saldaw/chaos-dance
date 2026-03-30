#include "Game.h"

#include "Menu.h"

Game::~Game() = default;
Game::Game()
    : main_window(sf::RenderWindow(sf::VideoMode::getDesktopMode(),
                                   "Chaos Dance", sf::Style::None)),
      player(std::make_shared<Player>(sf::Vector2{1, 1},
                                      PlayerConfig::VISIBILITY_RANGE,
                                      PlayerConfig::START_HP, grid)),
      message_text(font) {
  main_window.setFramerateLimit(60);
  if (Loader::loadFontFromResources(IDR_TTF1, font)) {
    message_text.setFont(font);
  }

  message_text.setStyle(sf::Text::Bold);
  message_text.setCharacterSize(180);
  message_text.setFillColor(sf::Color::Green);
  centerText(message_text);

  music_resource = Loader::loadMusicFromResources(IDR_RCDATA1);
  music_resource->music.setLooping(true);

  ui = std::make_unique<GameUI>();
  genMap();
  rhythm = std::make_unique<RhythmEngine>(RhythmConfig::BEATS_PER_MINUTE,
                                          RhythmConfig::HIT_WINDOW_SEC,
                                          RhythmConfig::COOLDOWN_PERCENT);

  current_state = GameState::Menu;
  main_menu = std::make_unique<Menu>(main_window, *this);
}

void Game::genMap() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist_x(MapConfig::MIN_WIDTH,
                                            MapConfig::MAX_WIDTH);
  std::uniform_int_distribution<int> dist_y(MapConfig::MIN_HEIGHT,
                                            MapConfig::MAX_HEIGHT);
  map_gen = std::make_unique<MapGenerator>(dist_x(gen), dist_y(gen));
  map_gen->generate();
  auto map = map_gen->getMap();
  unsigned int map_width = static_cast<unsigned int>(map[0].size());
  unsigned int map_hight = static_cast<unsigned int>(map.size());
  grid = std::make_shared<Grid>(map_width, map_hight, player);
  grid->updateSize(main_window.getSize());
  for (int y = 0; y < map.size(); ++y) {
    for (int x = 0; x < map[y].size(); ++x) {
      grid->setTile(x, y, map[y][x]);
    }
  }
  grid->moveObject(player, 1, 1);
  grid->spawnÑhest(MapConfig::NUM_OF_CHESTS);
  grid->spawnBombs(MapConfig::NUM_OF_BOMBS);
  grid->spawnKillers(MapConfig::NUM_OF_KILLERS);
}

void Game::setState(GameState state) {
  current_state = state;
  switch (state) {
    case GameState::Playing:
      music_resource->music.play();
      break;
    case GameState::Menu:
      music_resource->music.pause();
      break;
    default:
      break;
  }
}
void Game::restartGame(bool saveState) {
  grid = nullptr;
  int current_hp = player->getHp();
  level++;
  if (!saveState) {
    current_hp = PlayerConfig::START_HP;
    ui->resetCombo();
    level = 1;
  }

  ui->setLevel(level);
  player = std::make_shared<Player>(
      sf::Vector2{1, 1}, PlayerConfig::VISIBILITY_RANGE, current_hp, grid);
  genMap();
}
void Game::centerText(sf::Text& text) {
  sf::FloatRect bounds = text.getLocalBounds();
  text.setOrigin({bounds.size.x / 2, 0});
  text.setPosition({static_cast<float>(main_window.getSize().x) / 2.0f,
                    static_cast<float>(main_window.getSize().y) * 0.05f});
}

bool Game::canMoveHere(sf::Vector2i pos) {
  Tile::TileType type = grid->getTile(pos.x, pos.y);
  return grid->getTile(pos.x, pos.y) == Tile::TileType::kFloor &&
         grid->getObjectsAt(pos.x, pos.y).size() == 0;
}

void Game::handleEvents(float deltaTime) {
  while (const std::optional event = main_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) main_window.close();

    if (current_state == GameState::Menu) {
      main_menu->handleEvent(*event, deltaTime);
    } else {
      handleGameplayEvent(*event, deltaTime);
    }
  }
}

void Game::handleGameplayEvent(const sf::Event& event, float deltaTime) {
  if (auto keyEvent = event.getIf<sf::Event::KeyPressed>()) {
    // Îáðàáîòêà Escape
    if (keyEvent->code == sf::Keyboard::Key::Escape) {
      setState(GameState::Menu);
      return;
    }

    // Äâèæåíèå
    static const std::unordered_map<sf::Keyboard::Key, sf::Vector2i>
        keyToDirection = {{sf::Keyboard::Key::W, {0, -1}},
                          {sf::Keyboard::Key::S, {0, 1}},
                          {sf::Keyboard::Key::D, {1, 0}},
                          {sf::Keyboard::Key::A, {-1, 0}},
                          {sf::Keyboard::Key::Up, {0, -1}},
                          {sf::Keyboard::Key::Down, {0, 1}},
                          {sf::Keyboard::Key::Right, {1, 0}},
                          {sf::Keyboard::Key::Left, {-1, 0}}};

    auto it = keyToDirection.find(keyEvent->code);
    if (it != keyToDirection.end()) {
      attemptPlayerMove(it->second);
    }
  }
}

void Game::attemptPlayerMove(const sf::Vector2i& direction) {
  if (!rhythm->pressButton()) return;

  sf::Vector2i currentPos = player->getGridPosition();
  sf::Vector2i newPos = currentPos + direction;
  if (direction.x != 0) {
    player->flip(direction.x < 0);
  }
  if (grid->getCell(newPos.x, newPos.y)->getTileType() ==
      Tile::TileType::kExit) {
    gameWin();
    return;
  }
  if (canMoveHere(newPos)) {
    player->playAnimation("jump");
    grid->moveObject(player, newPos.x, newPos.y);
  } else {
    player->playAnimation("attack");
    Cell* cell = grid->getCell(newPos.x, newPos.y);
    for (std::shared_ptr<GameObject> obj : cell->getObjects()) {
      obj->getDamage(PlayerConfig::PLAYER_DAMAGE * (ui->getCombo() + 1));
      ui->addToCombo();
    }
  }
}

void Game::update(float deltaTime) {
  if (current_state == GameState::Menu) {
    main_menu->update(deltaTime);
  } else if (current_state == GameState::Playing) {
    player->update(deltaTime);
    grid->update(deltaTime);
    int pl_hp = player->getHp();
    ui->setHealth(pl_hp);
    if (pl_hp <= 0) gameOver();
    RhythmEngine::Movement beat_state = rhythm->update(deltaTime);
    if (beat_state != RhythmEngine::Movement::Null) {
      grid->beat();
      if (hits_to_reset_message > 0) hits_to_reset_message--;
      if (beat_state == RhythmEngine::Movement::PlayerNotClick) {
        player->playAnimation("idle");
        ui->resetCombo();
      }
    }
    grid->updateFogOfWar();
  }
}

void Game::render(float deltaTime) {
  main_window.clear(sf::Color::Black);

  if (current_state == GameState::Menu) {
    main_menu->render(deltaTime);
  } else {
    grid->draw(main_window);
    ui->draw(main_window);
    if (hits_to_reset_message > 0) {
      main_window.draw(message_text);
    }
  }
  main_window.display();
}
void Game::startGame() {
  sf::Clock clock;

  while (main_window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();
    handleEvents(deltaTime);
    update(deltaTime);
    render(deltaTime);
  }
}
void Game::gameOver() {
  hits_to_reset_message = OtherConfig::GAME_OVER_DISPLAY_BEATS;
  message_text.setFillColor(sf::Color::Red);
  message_text.setString("GAME OVER");
  centerText(message_text);
  restartGame(false);
}
void Game::gameWin() {
  hits_to_reset_message = OtherConfig::LEVEL_COMPLETE_DISPLAY_BEATS;
  message_text.setString("LEVEL completed");
  message_text.setFillColor(sf::Color::Green);
  centerText(message_text);
  restartGame(true);
}
