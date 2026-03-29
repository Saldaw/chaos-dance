#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Animation.h"

class Animator {
 public:
  Animator(sf::Sprite& sprite_) : sprite(sprite_) {}
  Animator(const Animator&) = delete;
  // Добавление анимации
  void addAnimation(const std::string& name, int frameWidth, int frameHeight,
                    int startX, int startY, int frameCount, float frameTime);

  // Переключение анимации
  void play(const std::string& name, bool loop = true);

  void stop();

  // Обновление текущей анимации
  void update(float deltaTime);

  // Проверка
  bool isPlaying() const;

  std::string getCurrentAnimation() const;

 private:
  sf::Sprite& sprite;
  std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
  std::optional<std::string> current_animation;
};
