#include "include/globalVar.hpp"

namespace Resolution {
	extern int w = 0;
	extern int h = 0;
};

namespace GameSettings {
	extern GameDificulty game_difficulty = GameDificulty::NORMAL;
	extern GameState game_state = GameState::MAIN_MENU;
	extern ControlsType controls_type = ControlsType::CONTROLS_NORMAL;
	extern int level_last = 1;
	extern int level_current = 1;
	extern unsigned int level_width = 0;
	extern unsigned int level_height = 0;
};
