#include <cmath>
//#include <iostream>
#include <memory>
#include "include/quick_functions.hpp"
#include "include/Blade.hpp"
#include "include/Camera.hpp"
#include "include/Character.hpp"
#include "include/Chimera.hpp"
#include "include/Zombie.hpp"
#include "include/UI.hpp"
#include "include/Menu.hpp"



int main()
{
	/////// "System" settings and utility def ///////
	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;		//	enabling AA
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "game.exe", sf::Style::Close, settings);
	window.setFramerateLimit(60);
	std::srand(std::time(0));
	sf::Clock clock; // creating clock obj
	float delta = 0.f;
	window.setKeyRepeatEnabled(false);	// информация считывается только в момент нажатия клавиши (зажатие игнорит)
	//	при использовании штучки типа "event.type == Event::Pressed && event.key.code == ..."

	std::unique_ptr<TextureStorage> txStorage = std::make_unique<TextureStorage>();

	/////// Game settings ///////
	GameState game_state = MAIN_MENU;
	GameDificulty game_difficulty = NORMAL;
	ControlsType controls_type = CONTROLS_NORMAL;
	bool first_time = true; // определяет первый цикл игрового процесса

	/////// Menus ///////
	MainMenu main_menu(12);
	SettingsMenu settings_menu(36, game_difficulty, controls_type);
	HelpMenu help_menu(18);
	std::unique_ptr<EndLevelMenu> end_level_menu;

	/////// level1 background ///////
	sf::Texture background;
	sf::Sprite sbackground;
	sbackground.setTexture(txStorage->GetTexture("Background"));
	sbackground.setScale({2,2});
	sbackground.setPosition(-1280.f, -1280.f);

	/////// UI ///////
	std::vector<std::unique_ptr<UIComponent>> UI;
	UI.push_back(std::make_unique<UIComponent>(txStorage->GetTexture("Bars"), BAR_HP, 0, 0, 13, 18, 4));
	UI.push_back(std::make_unique<UIComponent>(txStorage->GetTexture("Bars"), BAR_SP, 0, 18, 12, 15, 4));
	UI.push_back(std::make_unique<UIComponent>(txStorage->GetTexture("Bars"), BAR_ENEMY_COUNTER, 12, 22, 13, 11, 2));
	UI.push_back(std::make_unique<UIComponent>(txStorage->GetTexture("Icons"), ICON_ATTACK, 0, 0, 16, 16, 2));
	UI.push_back(std::make_unique<UIComponent>(txStorage->GetTexture("Icons"), ICON_CHARGE, 16, 0, 16, 16, 2));
	UI.push_back(std::make_unique<UIComponent>(txStorage->GetTexture("Icons"), ICON_POWERUP, 32, 0, 16, 16, 2));

	/////// Player, enemies, bullets and powerups ///////
	Character hero(txStorage->GetTexture("Character"), 0.f, 0.f, "player1", game_difficulty, controls_type);
	std::vector<std::unique_ptr<Enemy>> monsters;
	std::vector<std::unique_ptr<Bullet>> clip;
	std::vector<std::unique_ptr<Powerup>> loot;

	/////// Camera ///////
	std::unique_ptr<Camera> camera;
	camera = std::make_unique<Camera>(sf::Vector2f(CENTER_WINDOW_X, CENTER_WINDOW_Y), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	window.setView(camera->GetCamera());


	/////// Main cycle ///////
	while (window.isOpen())
	{
		sf::Event event;
		window.clear();

		delta = clock.restart().asSeconds();

		//// catch the resize events
		//if (event.type == sf::Event::Resized)
		//{
		//	// update the view to the new size of the window
		//	sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
		//	camera.Resize(visibleArea);
		//}

		switch (game_state) {

		case MAIN_MENU:
		{
			camera->SetMenuPos();
			window.setView(camera->GetCamera());
			main_menu.Animation(MouseWrldPos(window));


			while (window.pollEvent(event)){
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
				main_menu.update(game_state, window, event);
			}
			window.draw(main_menu);
			break;
		}

		case SETTINGS_MENU: {
			settings_menu.Animation(MouseWrldPos(window));
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
				settings_menu.update(game_state, window, event);
			}

			hero.SetControls(controls_type);

			window.draw(settings_menu);
			break;
		}

		case HELP_MENU: {
			help_menu.Animation(MouseWrldPos(window));
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
				help_menu.update(game_state, window, event);
			}
			window.draw(help_menu);
			break;
		}

		case LEVEL1:
		{
			// restarting level preparation
			if(first_time){
				hero.SetDifficulty(game_difficulty);
				hero.SetPosition(0, 0);
				clip.clear();
				loot.clear();
				monsters.clear();

				// spawning monsters;
				for (size_t i = 0; i < ZOMBIE_COUNT; i++) {
					monsters.push_back(std::make_unique<Zombie>(txStorage->GetTexture("Zombie"), "zombie"));
				}
				for (size_t i = 0; i < CHIMERA_COUNT; i++) {
					monsters.push_back(std::make_unique<Chimera>(txStorage->GetTexture("Chimera"), "chimera"));
				}


				first_time = false;
			}
			// handling one-time key-pressing events
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					game_state = MAIN_MENU;
				}
				hero.Charge(delta, event);
				if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
					hero.Attack();
				}
			}

			window.draw(sbackground);

			hero.update(delta, window);

			// updating monsters (movement, interaction with a player, drawing)
			for (size_t i = 0; i < monsters.size(); i++) {

				/////// Player attacks ///////
				if (hero.attacking && GetFastDistanceBP(hero.GetCords(), monsters[i]->GetCords()) < 5000) {
					sf::Vector2f hero_to_enemy = Normalize2(monsters[i]->GetCords() - hero.GetCords());
					if (GetAngleBV2(hero.GetFacingVec(), hero_to_enemy) < 1.31) {
						monsters[i]->RecieveDamage(hero.GetDmg());
						camera->SetShake();
						//					std::cout << "SMELL THE BEBRA!\n";
					}
				}
				/////// Enemies update ///////
				monsters[i]->update(delta, hero);
				if (monsters[i]->GetName() == "chimera") {
					if (*(monsters[i]->GetState()) == ATTACKING) {
						clip.push_back(std::make_unique<Bullet>(monsters[i]->GetCords().x, monsters[i]->GetCords().y, monsters[i]->GetFacingVec()));
						*(monsters[i]->GetState()) = IDLE;
					}
				}

				if (*(monsters[i]->GetState()) == DEAD) {
					if (!(std::rand() % 4))	loot.push_back(std::make_unique<Powerup>(monsters[i]->GetCords()));
					monsters.erase(monsters.begin() + i);
					i--;
				}
				else window.draw(*monsters[i]);
			}

			hero.attacking = false;

			//updating bullets (lifetime, interaction with a player)
			for (size_t i = 0; i < clip.size(); i++) {
				clip[i]->update(hero);
				window.draw(clip[i]->sprite);
				if (clip[i]->type == DYING) {
					clip.erase(clip.begin() + i);
					i--;
				}
				else if (clip[i]->type == DAMAGING) {
					hero.RecieveDmg(clip[i]->dmg);
					camera->SetRadialShake();
					clip.erase(clip.begin() + i);
					i--;
				}
			}

			// updating powerups (lifetime, interaction with a player)
			for (size_t i = 0; i < loot.size(); i++) {
				window.draw(*loot[i]);
				if (GetFastDistanceBP(hero.GetCords(), loot[i]->GetCords()) < 740) {
					hero.PickUpPowerup(loot[i]->GetType());
					loot.erase(loot.begin() + i);
					i--;
				}
				else if (loot[i]->IsDead()) {
					loot.erase(loot.begin() + i);
					i--;
				}
			}

			/////// Collecting info about  ///////
			hero.SetEnemiesAmount(monsters.size());

			camera->update(hero.GetCords(), window);

			window.setView(camera->GetCamera());

//			window.draw(sword);
			window.draw(hero);

			// updating and drawing UI
			for (size_t i = 0; i < UI.size(); i++) {
				UI[i]->update(camera->GetPos(), hero);
				window.draw(*UI[i]);
			}
			// is player dead?
			if (hero.GetHP().y < 1) {
				game_state = DEFEAT;
			}
			// are all the monsters dead?
			if (monsters.size() == 0) {
				game_state = VICTORY;
			}

			break;
		}
		default: {
			if (!first_time) {
				camera->SetMenuPos();
				window.setView(camera->GetCamera());
				end_level_menu = std::make_unique<EndLevelMenu>(36, game_state);
				first_time = true;
			}
			end_level_menu->Animation(MouseWrldPos(window));
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
				end_level_menu->update(game_state, window, event);
			}
			window.draw(*end_level_menu);
			break;
		}
		}
		window.display();
	}

	return 0;
}