#pragma once

#if _DEBUG

#define MEMORY_TRACKING 0 // 1 = default, 2 = verbose
#define IGNORE_UNKNOWNS 0

// Camera
#define CAMERA_IGNORE_BOUNDARIES 0
#define PRINT_SHAKEYCAM_VALUES 0
#define SET_GAME_SCALE 0 // 
constexpr float game_scale = 0.5f;


// Framerate
#define FRAMERATE_CAP 0 // Use the FPS cap you want
#define PRINT_FRAMERATE_EVERY 0 // Print at the rate you specify in milliseconds i.e. 500 = print every 0.5 seconds

// Player
#define DRAW_PLAYER_RECT 0
#define DRAW_PLAYER_WEAPON_RECT 0
#define IGNORE_WALLS 0
#define IGNORED_BY_ENEMIES 0

// Abilities
#define UNLOCK_ALL_ABILITIES 0
#define NO_ABILITY_COOLDOWNS 1
#define DRAW_ABILITY_RECTS 0

// Collectables
#define DRAW_COLLECTABLE_RECT 0


// Map
#define MAP_BOUNDARIES 0
#define LABEL_TILE_INDEX 0
#define LABEL_SURFACE_RENDER_TYPES 0
#define LABEL_SURFACE_COLLISION_TYPES 0
#define LABEL_SURFACE_DECOR_TYPES 0
#define RENDER_SURFACE_TYPES (LABEL_SURFACE_RENDER_TYPES ||  LABEL_SURFACE_COLLISION_TYPES || LABEL_SURFACE_DECOR_TYPES || LABEL_TILE_INDEX)

// Enemy / AI
#define DRAW_AI_PATH 0
#define DRAW_AI_PATH_COSTMAP 0

#define LABEL_ENEMY_STATES 0
#define LABEL_ENEMY_HEALTH 0
#define DRAW_PLAYER_ENEMY_DISTANCE 0

#define DRAW_ENEMY_RECTS 0
#define DRAW_ENEMY_TARGET_PATH 0

#define LIMIT_ENEMY_SPAWNS 1
#define MAX_SPAWN_COUNT 1

// UI
#define UI_EDITOR 1
#define DRAW_UI_RECTS 1

#define DEBUG_CURSOR 0


// Collisions
#define TRACK_COLLISIONS 0


// Audio
#define PRINT_PLAY_AUDIO 0

#else
#define PRINT_FRAMERATE_EVERY 1000 // Print at the rate you specify in milliseconds i.e. 500 = print every 0.5 seconds

#define MEMORY_TRACKING 0

#endif
