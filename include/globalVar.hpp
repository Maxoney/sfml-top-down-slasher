#pragma once
#include "SFML/Graphics.hpp"
#include "include/TextureStorage.hpp"
#include <chrono>


constexpr float M_PI_4 = 0.785398163397448309616;  // pi/4
constexpr int WINDOW_WIDTH = 900;
constexpr int WINDOW_HEIGHT = 600;
constexpr int CENTER_WINDOW_X = WINDOW_WIDTH / 2;
constexpr int CENTER_WINDOW_Y = WINDOW_HEIGHT / 2;
constexpr size_t ZOMBIE_COUNT = 15;
constexpr size_t CHIMERA_COUNT = 7;
constexpr auto FONT_LOCATION_FFF_FORWARD = "res/fonts/fff_forward.ttf";




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

enum GameDificulty {
	EASY,
	NORMAL,
	HARD
};

enum GameState {
	MAIN_MENU,
	SETTINGS_MENU,
	HELP_MENU,
	LEVEL1,
	DEFEAT,
	VICTORY
};

enum ControlsType {
	CONTROLS_NORMAL,
	CONTROLS_FIXED
};

enum PowerupType {
	NONE,
	INVINCIBILITY,
	ENERGY_BOOST
};