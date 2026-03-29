#include "RhythmEngine.h"

RhythmEngine::RhythmEngine(float beatsPerMinute, float hitWindowSec,
                           float cooldownPercent)
    : bpm(beatsPerMinute),
      hitWindow(hitWindowSec),
      timer(0.f),
      cooldownPercent(cooldownPercent),
      cooldownTimer(0),
      onCooldown(false),
      canHit(false),
      wasHit(true) {
  beatDuration = 60.0f / bpm;
}

RhythmEngine::Movement RhythmEngine::update(float deltaTime) {
  timer += deltaTime;

  if (onCooldown) {
    cooldownTimer += deltaTime;
    if (cooldownTimer >= beatDuration * cooldownPercent) {
      onCooldown = false;
      cooldownTimer = 0;
    }
  }

  if (timer >= beatDuration) {
    timer -= beatDuration;
    if (timer < beatDuration) {
      wasHit = false;
      canHit = true;
    }
  }
  if (timer > hitWindow) {
    if (canHit) {
      if (!wasHit) {
        canHit = false;
        return Movement::PlayerNotClick;
      }
    } else if (wasHit) {
      wasHit = false;
      canHit = false;
      return Movement::NewTact;
    }
  }

  return Movement::Null;
}

bool RhythmEngine::pressButton() {
  if (canHit && !onCooldown) {
    if (!wasHit) {
      wasHit = true;
      canHit = false;
      cooldownTimer = 0;
      return true;
    }
  }
  onCooldown = true;
  cooldownTimer = 0.f;
  return false;
}
