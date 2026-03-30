#pragma once

namespace RhythmConfig {
inline constexpr float BEATS_PER_MINUTE = 90.0f;
inline constexpr float HIT_WINDOW_SEC = 0.3f;
inline constexpr float COOLDOWN_PERCENT = 0.5f;
}  // namespace RhythmConfig

namespace EnemyConfig {
inline constexpr int VISIBILITY_RANGE_ENEMY = 4;
// BombCake
inline constexpr int BOMB_DAMAGE = 1;
inline constexpr int BOMB_HP = 1;
inline constexpr int STEPS_TO_EXPLOSION = 4;
inline constexpr int DIST_EXPLOSION = 1;
// Killer
inline constexpr int KILLER_DAMAGE = 1;
inline constexpr int KILLER_HP = 4;
}  // namespace EnemyConfig

namespace PlayerConfig {
inline constexpr int MAX_HP = 4;
inline constexpr int START_HP = 2;
inline constexpr int VISIBILITY_RANGE = 3;
inline constexpr int PLAYER_DAMAGE = 1;
}  // namespace PlayerConfig

namespace MapConfig {
inline constexpr int MIN_WIDTH = 7;
inline constexpr int MAX_WIDTH = 20;
inline constexpr int MIN_HEIGHT = 5;
inline constexpr int MAX_HEIGHT = 15;
inline constexpr int WALL_THICKNESS = 1;
inline constexpr float WALL_DENSITY = 0.25f;  // 25% стен внутри
inline constexpr int MIN_WALL_LENGTH = 3;
inline constexpr int MAX_WALL_LENGTH = 8;

inline constexpr int NUM_OF_CHESTS = 2;
inline constexpr int NUM_OF_BOMBS = 1;
inline constexpr int NUM_OF_KILLERS = 1;
}  // namespace MapConfig

namespace OtherConfig {
inline constexpr int LEVEL_COMPLETE_DISPLAY_BEATS = 3;
inline constexpr int GAME_OVER_DISPLAY_BEATS = 3;
}
