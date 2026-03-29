#include "Animator.h"
void Animator::addAnimation(const std::string& name, int frameWidth,
                            int frameHeight, int startX, int startY,
                            int frameCount, float frameTime) {
  auto animation = std::make_unique<Animation>();
  animation->create(sprite, frameWidth, frameHeight, startX, startY, frameCount,
                    frameTime);

  animations[name] = std::move(animation);
}

void Animator::play(const std::string& name, bool loop) {
  // Останавливаем текущую
  if (current_animation && current_animation != name) {
    animations[current_animation.value()]->stop();
  }
  if (animations.find(name) != animations.end()) {
    current_animation = name;
    animations[name]->play(loop);
  }
}

void Animator::stop() {
  if (current_animation) {
    animations[current_animation.value()]->stop();
    current_animation.reset();
  }
}

void Animator::update(float deltaTime) {
  if (current_animation) {
    animations[current_animation.value()]->update(deltaTime);
  }
}

bool Animator::isPlaying() const {
  return current_animation.has_value() &&
         animations.at(current_animation.value())->isPlaying();
}

std::string Animator::getCurrentAnimation() const {
  return current_animation.value_or("");
}
