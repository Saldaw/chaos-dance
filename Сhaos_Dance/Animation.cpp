#include "Animation.h"
void Animation::create(sf::Sprite& sprite_, int frameWidth, int frameHeight,
                       int startX, int startY, int frameCount,
                       float frameTime) {
  frames.clear();
  for (int i = 0; i < frameCount; ++i) {
    sf::IntRect rect;
    rect.position.x = startX + i * frameWidth;
    rect.position.y = startY;
    rect.size.x = frameWidth;
    rect.size.y = frameHeight;
    frames.push_back(rect);
  }

  frame_time = frameTime;
  current_frame = 0;
  timer = 0.0f;
  sprite = &sprite_;
  if (!frames.empty()) {
    sprite_.setTextureRect(frames[1]);
  }
}

void Animation::update(float deltaTime) {
  if (!is_playing || frames.empty()) return;

  timer += deltaTime;
  if (timer >= frame_time) {
    timer = 0.0f;
    current_frame++;

    if (current_frame >= frames.size()) {
      if (is_looping) {
        current_frame = 0;
      } else {
        current_frame = static_cast<int>(frames.size()) - 1;
        is_playing = false;
      }
    }

    if (sprite && current_frame < frames.size()) {
      sprite->setTextureRect(frames[current_frame]);
    }
  }
}

void Animation::play(bool loop) {
  is_playing = true;
  current_frame = 0;
  sprite->setTextureRect(frames[current_frame]);
  is_looping = loop;
}

void Animation::stop() {
  Animation::is_playing = false;
  current_frame = 0;
  if (sprite && !frames.empty()) {
    sprite->setTextureRect(frames[0]);
  }
}
