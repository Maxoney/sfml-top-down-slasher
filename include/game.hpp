#pragma once
#include <SFML/Graphics.hpp>
#include "include/globalVar.hpp"
#include "include/level.hpp"
#include "include/quick_functions.hpp"
#include "include/Blade.hpp"
#include "include/Camera.hpp"
#include "include/Character.hpp"
#include "include/Chimera.hpp"
#include "include/Zombie.hpp"
#include "include/UI.hpp"
#include "include/Menu.hpp"
#include "include/level.hpp"



class Game {
public:
	Game() = default;
	~Game();

	void Init();	//	"супер €вный конструктор" важных компонентов

	bool Tick();


private:
	/////// Game settings and useful stuff ///////
	sf::ContextSettings* context_settings;
	sf::RenderWindow* window;
	sf::Clock* clock; // creating clock obj
	sf::Event* event;
	TextureStorage* txStorage;
	Camera* camera;

	/////// Menus ///////
	MainMenu* main_menu;
	SettingsMenu* settings_menu;
	HelpMenu* help_menu;
	std::unique_ptr<EndLevelMenu> end_level_menu;

	

	Character* character;
	Level* level;
	
	bool level_initialized = 0;

	//float delta = 0.f;
};