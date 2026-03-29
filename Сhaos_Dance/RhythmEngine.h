#pragma once
class RhythmEngine {
 private:
  float bpm;
  float beatDuration;     // Секунд на удар
  float timer;            // Таймер от начала текущего такта
  float hitWindow;        // Окно попадания (в секундах)
  float cooldownPercent;  // КД в процентах от такта
  float cooldownTimer;    // Таймер КД
  bool onCooldown;        // Флаг КД
  bool canHit;            // Можно ли сейчас попасть?
  bool wasHit;            // Был ли уже удар в этом такте?

 public:
  enum class Movement { PlayerNotClick, NewTact, Null };
  RhythmEngine(float beatsPerMinute, float hitWindowSec, float cooldownPercent);

  Movement update(float deltaTime);
  bool pressButton();  // Проверка удачности нажатия
};
