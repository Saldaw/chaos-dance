#include "Game.h"

#include "Menu.h"

Game::~Game() = default;
Game::Game()
    : main_window(sf::RenderWindow(sf::VideoMode::getDesktopMode(),
                                   "Chaos Dance", sf::Style::Default)),
      player(std::make_shared<Player>(sf::Vector2{1, 1},
                                      PlayerConfig::VISIBILITY_RANGE, grid)),
      result_text(font) {
  main_window.setFramerateLimit(60);
  current_state = GameState::Menu;
  if (Loader::loadFontFromResources(IDR_TTF1, font)) {
    result_text.setFont(font);
  }
  result_text.setStyle(sf::Text::Bold);
  sf::FloatRect bounds = result_text.getLocalBounds();
  result_text.setOrigin({bounds.size.x / 2, bounds.size.y / 2});

  result_text.setCharacterSize(360);
  result_text.setPosition({0, 400});

  music_resource = Loader::loadMusicFromResources(IDR_RCDATA1);
  music_resource->music.setLooping(true);

  ui = std::make_unique<GameUI>();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist_x(MapConfig::MIN_WIDTH,
                                            MapConfig::MAX_WIDTH);
  std::uniform_int_distribution<int> dist_y(MapConfig::MIN_HEIGHT,
                                            MapConfig::MAX_HEIGHT);
  map_gen = std::make_unique<MapGenerator>(dist_x(gen), dist_y(gen));
  map_gen->generate();
  auto map = map_gen->getMap();

  grid = std::make_shared<Grid>(map[0].size(), map.size(), player);
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

  rhythm = std::make_unique<RhythmEngine>(RhythmConfig::BEATS_PER_MINUTE,
                                          RhythmConfig::HIT_WINDOW_SEC,
                                          RhythmConfig::COOLDOWN_PERCENT);
  main_menu = std::make_unique<Menu>(main_window, *this);
}

void Game::setState(GameState state) {
  current_state = state;
  switch (state) {
    case GameState::Playing:
      music_resource->music.play();
      break;
    default:
      break;
  }
}

bool Game::canMoveHere(sf::Vector2i pos) {
  Tile::TileType type = grid->getTile(pos.x, pos.y);
  if (type == Tile::TileType::kExit) {
    gameWin();
  }
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
  } else if (is_active) {
    player->update(deltaTime);
    grid->update(deltaTime);
    int pl_hp = player->getHp();
    ui->setHealth(pl_hp);
    if (pl_hp <= 0) gameOver();
    RhythmEngine::Movement beat_state = rhythm->update(deltaTime);
    if (beat_state != RhythmEngine::Movement::Null) {
      grid->beat();
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
    if (!is_active) {
      main_window.draw(result_text);
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
void Game::openMenu() {}
void Game::gameOver() {
  is_active = false;
  result_text.setFillColor(sf::Color::Red);
  result_text.setString("GAME OVER");
}
void Game::gameWin() {
  is_active = false;
  result_text.setFillColor(sf::Color::Green);
  result_text.setString("VICTORY!");
}
