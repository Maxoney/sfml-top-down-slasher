#include "..\include\game.hpp"

Game::~Game()
{
	delete event;
	delete camera;
	delete level;
	delete character;
	delete help_menu;
	delete settings_menu;
	delete main_menu;
	delete txStorage;
	delete window;
	delete context_settings;
	delete clock;
}

void Game::Init()
{
	///////	Utility ///////
	clock = new sf::Clock();
	Resolution::h = 600;
	Resolution::w = 900;

	context_settings = new sf::ContextSettings();
	context_settings->antialiasingLevel = 2;		//	enabling AA

	window = new sf::RenderWindow(sf::VideoMode(Resolution::w, Resolution::h), "game.exe", sf::Style::Close, *context_settings);
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(false);	// информация считывается только в момент нажатия клавиши (зажатие игнорит)

	std::srand(std::time(0));	//	updating rand seed

	txStorage = new TextureStorage();

	/////// Game settings ///////
	GameSettings::game_state = MAIN_MENU;
	GameSettings::game_difficulty = NORMAL;
	GameSettings::controls_type = CONTROLS_NORMAL;


	///////	Game menu ///////
	main_menu = new MainMenu(12);
	settings_menu = new SettingsMenu(36);
	help_menu = new HelpMenu(18);

	///////	Character ///////
	character = new Character(txStorage->GetTexture("Character"), 0.f, 0.f, window, &delta);

	/////// Camera ///////
	camera = new Camera(character->GetCords(), window);
	window->setView(camera->GetCamera());

	///////	Level ///////
	level = new Level(txStorage, character, camera, &delta);
	level->Init();

	event = new sf::Event();
	//	нужно для инициализации уровня. после имплементации класса "Level" удалить
	bool first_time = true; // определяет первый цикл игрового процесса
}

bool Game::Tick()
{
	//sf::Event event;
	//window->pollEvent(*event);

	window->clear();
	delta = clock->restart().asSeconds();

	
	// catch the resize events
	if (event->type == sf::Event::Resized)
	{
		// update the view to the new size of the window
		sf::FloatRect visibleArea(0.f, 0.f, event->size.width, event->size.height);
		camera->Resize(visibleArea);
	}

	
	switch (GameSettings::game_state) {

	case GameState::MAIN_MENU:
	{
		camera->SetMenuPos();
		window->setView(camera->GetCamera());
		main_menu->Animation(MouseWrldPos(*window));


		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::Closed) window->close();
			main_menu->update(*window, *event);
		}
		window->draw(*main_menu);
		break;
	}
	case GameState::SETTINGS_MENU: {
		settings_menu->Animation(MouseWrldPos(*window));
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::Closed) window->close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) GameSettings::game_state = GameState::MAIN_MENU;
			settings_menu->update(*window, *event);
		}

		character->updateControls();

		window->draw(*settings_menu);
		break;
	}
	case GameState::HELP_MENU: {
		help_menu->Animation(MouseWrldPos(*window));
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::Closed) window->close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) GameSettings::game_state = GameState::MAIN_MENU;
			help_menu->update(*window, *event);
		}
		window->draw(*help_menu);
		break;
	}
	case GameState::GAME: {
		// handling one-time key-pressing events
		while (window->pollEvent(*event))
		{
			if (event->type == sf::Event::Closed) window->close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) GameSettings::game_state = GameState::MAIN_MENU;
			character->Controls(event);
		}

		if (level->update()) {
			if (GameSettings::level_current < GameSettings::level_last) {
				GameSettings::level_current++;
				level->Init();
			}
			GameSettings::game_state = GameState::VICTORY;
			end_level_menu = std::make_unique<EndLevelMenu>(36);
		}
		else if (character->GetHP()->y < 1) {
			GameSettings::game_state = GameState::DEFEAT;
			end_level_menu = std::make_unique<EndLevelMenu>(36);
		}


		window->draw(*level);


		camera->update();
		window->setView(camera->GetCamera());

		break;
	}
	default: {

		camera->SetMenuPos();
		window->setView(camera->GetCamera());

		end_level_menu->Animation(MouseWrldPos(*window));
		while (window->pollEvent(*event)) {
			if (event->type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window->close();
			end_level_menu->update(*window, *event);
		}
		window->draw(*end_level_menu);
		break;
	}

	}
	window->display();
	return window->isOpen();
}

