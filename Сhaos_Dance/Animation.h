#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
 public:
  Animation() = default;

  // Создание анимации из спрайт-листа
  void create(sf::Sprite& sprite_, int frameWidth, int frameHeight, int startX,
              int startY, int frameCount, float frameTime);

  // Обновление анимации (вызывать каждый кадр)
  void update(float deltaTime);

  // Управление
  void play(bool loop = true);

  void stop();

  void pause() { is_playing = false; }

  void resume() { is_playing = true; }

  // Проверка состояния
  bool isPlaying() const { return is_playing; }
  int getCurrentFrame() const { return current_frame; }

 private:
  sf::Sprite* sprite = nullptr;
  std::vector<sf::IntRect> frames;
  float frame_time = 0.1f;  // Время на кадр
  float timer = 0.0f;
  int current_frame = 0;
  bool is_playing = false;
  bool is_looping = true;
};
