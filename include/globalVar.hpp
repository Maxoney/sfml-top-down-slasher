#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include "include/TextureStorage.hpp"

class Enemy;
class Bullet;
class Powerup;
class UIComponent;

using vec_uptrEnemy = std::vector<std::unique_ptr<Enemy>>;
using vec_uptrBullet = std::vector<std::unique_ptr<Bullet>>;
using vec_uptrPowerup = std::vector<std::unique_ptr<Powerup>>;
using vec_uptrUIComponent = std::vector<std::unique_ptr<UIComponent>>;


constexpr float M_PI_4 = 0.785398163397448309616;  // pi/4
constexpr int WINDOW_WIDTH = 900;
constexpr int WINDOW_HEIGHT = 600;
constexpr int CENTER_WINDOW_X = WINDOW_WIDTH / 2;
constexpr int CENTER_WINDOW_Y = WINDOW_HEIGHT / 2;
constexpr size_t ZOMBIE_COUNT = 15;
constexpr size_t CHIMERA_COUNT = 7;
constexpr auto FONT_LOCATION_FFF_FORWARD = "res/fonts/fff_forward.ttf";
static float delta = 0.f;

namespace Resolution {
	extern int w,	//	width
			   h;	//	height
};

enum GameDificulty {
	EASY,
	NORMAL,
	HARD
};

enum GameState {
	MAIN_MENU,
	SETTINGS_MENU,
	HELP_MENU,
	GAME,
	DEFEAT,
	VICTORY
};

enum ControlsType {
	CONTROLS_NORMAL,
	CONTROLS_FIXED
};

namespace GameSettings {
	extern GameDificulty game_difficulty;
	extern GameState game_state;
	extern ControlsType controls_type;
	extern int level_last;
	extern int level_current;
	extern unsigned int level_width;
	extern unsigned int level_height;
};


enum CreatureState {	// состояния для будущей реализации анимаций
	IDLE,
	WALKING_FORWARD,
	RUNNING_FORWARD,
	WALKING_BACK,
	RUNNING_BACK,
	WALKING_RSIDE,
	RUNNING_RSIDE,
	WALKING_LSIDE,
	RUNNING_LSIDE,
	WALKING,
	ATTACKING,
	DEAD
};


enum PowerupType {
	NONE,
	INVINCIBILITY,
	ENERGY_BOOST
};