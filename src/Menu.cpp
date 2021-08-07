#include "..\include\Menu.hpp"

MenuButton::MenuButton(MenuButtonType _type, const std::string str, const int & font_size, const sf::Vector2f & cords)
{
	type = _type;
	if (type == IMAGE) {
		texture.loadFromFile(str);
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);
	}
	else {
		font.loadFromFile(FONT_LOCATION_FFF_FORWARD);
		text.setFont(font);
		text.setCharacterSize(font_size);
		text.setString(str);
		text.setPosition(cords);
		boundingBox.setSize(sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height));
		boundingBox.setPosition(text.getGlobalBounds().left, text.getGlobalBounds().top);
	}
}

void MenuButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(type == IMAGE) target.draw(sprite, states);
	else target.draw(text, states);
}

MenuButtonType* MenuButton::GetType()
{
	return &type;
}

sf::RectangleShape* MenuButton::GetBoundingBox()
{
	return (&boundingBox);
}

bool MenuButton::Fading(const sf::Vector2f & mouse_pos)
{
	// if object is clickabe / interactive
	if (this->type != IMAGE && this->type != TEXT) {
		// if mouse is on text
		if (boundingBox.getGlobalBounds().contains(mouse_pos)) {
			// if text hasn't faded yet
			if (text.getFillColor() != sf::Color(155, 155, 155, 255)) text.setFillColor(text.getFillColor() - sf::Color(10, 10, 10, 0));
			return true;
		}
		else text.setFillColor(sf::Color(255, 255, 255));
	}
	return false;
}

sf::Sprite * MenuButton::GetSprite()
{
	return &sprite;
}

sf::Texture * MenuButton::GetTexture()
{
	return &texture;
}

sf::Text * MenuButton::ChangeText()
{
	return &text;
}

void Menu::Animation(const sf::Vector2f & mouse_pos)
{
	for (size_t i = 0; i < menu.size(); i++) {
		intersects[i] = menu[i]->Fading(mouse_pos);
	}
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (size_t i = 0; i < menu.size(); i++) {
		menu[i]->draw(target, states);
	}
}

MainMenu::MainMenu(const int& font_size)
{
	// ������ ����� � �����
	int side_indent = 42,
		bottom_indent = WINDOW_HEIGHT - side_indent - font_size * 1.8 * 4; // 4- ammount of menu sections
	// Play button
	menu.push_back(std::make_unique<MenuButton>(PLAY, "Start game", font_size, sf::Vector2f(side_indent, bottom_indent)));
	// Settings button
	menu.push_back(std::make_unique<MenuButton>(SETTINGS, "Settings", font_size, sf::Vector2f(side_indent, bottom_indent + menu.size() * font_size * 1.8)));
	// Help button
	menu.push_back(std::make_unique<MenuButton>(HELP, "Help", font_size, sf::Vector2f(side_indent, bottom_indent + menu.size() * font_size * 1.8)));
	// Exit button
	menu.push_back(std::make_unique<MenuButton>(EXIT, "Exit game", font_size, sf::Vector2f(side_indent, bottom_indent + menu.size() * font_size * 1.8)));

	for (size_t i = 0; i < menu.size(); i++) {
		intersects.push_back(false);
	}
}

void MainMenu::update(GameState & game_state, sf::RenderWindow & window, const sf::Event & event)
{
	for (size_t i = 0; i < menu.size(); i++) {
		if (intersects[i] && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
//		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left && menu[i]->GetBoundingBox()->getGlobalBounds().contains(mouse_pos)) {
			switch (*(menu[i]->GetType()))
			{
			case PLAY: game_state = LEVEL1; break;
			case SETTINGS: game_state = SETTINGS_MENU; break;
			case HELP: game_state = HELP_MENU; break;
			case EXIT: window.close(); break;
			}
		}
	}
}

SettingsMenu::SettingsMenu(const int& font_size, GameDificulty& diff, ControlsType& contr)
{
	difficulty = &diff;
	controls = &contr;
	int side_indent = 120,
		upper_indent = side_indent; // 4- ammount of menu sections
	// Controls type change
	menu.push_back(std::make_unique<MenuButton>(SETTINGS_CONTROLS, "Controls:  NORMAL", font_size, sf::Vector2f(side_indent, upper_indent)));
	// Difficulty change
	menu.push_back(std::make_unique<MenuButton>(SETTINGS_DIFFICULTY, "Difficulty:  NORMAL", font_size, sf::Vector2f(side_indent, upper_indent + menu.size() * font_size * 3)));
	// "Back" button
	menu.push_back(std::make_unique<MenuButton>(BACK, "Back", font_size, sf::Vector2f(64, WINDOW_HEIGHT - 64 - font_size)));

	for (size_t i = 0; i < menu.size(); i++) {
		intersects.push_back(false);
	}
}

void SettingsMenu::update(GameState & game_state, sf::RenderWindow & window, const sf::Event & event)
{
	for (size_t i = 0; i < menu.size(); i++) {
		if (intersects[i] && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			//		if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left && menu[i]->GetBoundingBox()->getGlobalBounds().contains(mouse_pos)) {
			switch (*(menu[i]->GetType()))
			{
			case SETTINGS_CONTROLS: {
				auto str = [this]() mutable -> std::string {
					if (*controls == CONTROLS_FIXED) {
						*controls = CONTROLS_NORMAL;
						return "NORMAL";
					}
					else {
						*controls = CONTROLS_FIXED;
						return "FIXED";
					}
				};

				menu[i]->ChangeText()->setString("Controls:  " + str());
				break;
			}
			case SETTINGS_DIFFICULTY: {
				auto str = [this]() mutable -> std::string {
					if (*difficulty == EASY) {
						*difficulty = NORMAL;
						return "NORMAL";
					}
					else if (*difficulty == NORMAL) {
						*difficulty = HARD;
						return "MY>|<UK";
					}
					else {
						*difficulty = EASY;
						return "EASY";
					}
				};

				menu[i]->ChangeText()->setString("Difficulty:  " + str());
				break;
			}
			case BACK: game_state = MAIN_MENU; break;
			}
		}
	}

}

HelpMenu::HelpMenu(const int & font_size)
{
	// ������ ����� � �����
	int side_indent = 16,
		bottom_indent = WINDOW_HEIGHT - side_indent - font_size - 5;

	// IMAGES FIRST (to draw properly)
	menu.push_back(std::make_unique<MenuButton>(IMAGE, "res/sprites/help0.png", font_size, sf::Vector2f(0, 0)));
	// Previous page button (currently TEXT because on the first page it is unclickable)
	menu.push_back(std::make_unique<MenuButton>(TEXT, "Previous", font_size, sf::Vector2f(side_indent, bottom_indent)));
	menu[menu.size()-1]->ChangeText()->setFillColor(sf::Color(100, 100, 100, 255));
	// Next page button
	menu.push_back(std::make_unique<MenuButton>(NEXT, "Next", font_size, sf::Vector2f(WINDOW_WIDTH-side_indent-64, bottom_indent)));
	// Back to Main Menu button
	menu.push_back(std::make_unique<MenuButton>(BACK, "BACK", font_size, sf::Vector2f(CENTER_WINDOW_X-36, bottom_indent+10)));

	for (size_t i = 0; i < menu.size(); i++) {
		intersects.push_back(false);
		p_textures.push_back(menu[i]->GetTexture());
		p_sprites.push_back(menu[i]->GetSprite());
	}
}

void HelpMenu::update(GameState & game_state, sf::RenderWindow & window, const sf::Event & event)
{
	for (size_t i = 0; i < menu.size(); i++) {
		if (intersects[i] && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			switch (*(menu[i]->GetType()))
			{
			case BACK: game_state = MAIN_MENU; break;
			case PREVIOUS:
				if (menu_page == 1) { 
					menu_page--; 
					menu[i]->ChangeText()->setFillColor(sf::Color(100, 100, 100, 255));
					*(menu[i]->GetType()) = TEXT;
					p_textures[0]->loadFromFile("res/sprites/help0.png"); 
				}
				else if (menu_page == 2) { 
					menu_page--; 
					// Changing NEXT color to normal. (NEXT goes after PREVIOUS)
					menu[i + 1]->ChangeText()->setFillColor(sf::Color(255, 255, 255, 255));
					*(menu[i + 1]->GetType()) = NEXT;
					p_textures[0]->loadFromFile("res/sprites/help1.png");
				}
				break;
			case NEXT:
				if (menu_page == 0) {  
					// Changing PREVIOUS color to normal. (PREVIOUS goes before NEXT)
					menu[i - 1]->ChangeText()->setFillColor(sf::Color(255, 255, 255, 255));
					*(menu[i - 1]->GetType()) = PREVIOUS;
					p_textures[0]->loadFromFile("res/sprites/help1.png");
					menu_page++; 
				}
				else if (menu_page == 1) { 
					menu_page++;
					p_textures[0]->loadFromFile("res/sprites/help2.png");
					menu[i]->ChangeText()->setFillColor(sf::Color(100, 100, 100, 255));
					*(menu[i]->GetType()) = TEXT;
				}
				break;
			}
			p_sprites[0]->setTexture(*p_textures[0]);
		}
	}
}

EndLevelMenu::EndLevelMenu(const int & font_size, GameState & game_state)
{
	int side_indent = 120,
		upper_indent = side_indent;
	std::string str_state;
	if (game_state == DEFEAT) {
		str_state = "You lost :(";
	} else str_state = "You won!";
	// Controls type change
	menu.push_back(std::make_unique<MenuButton>(TEXT, str_state, font_size, sf::Vector2f(CENTER_WINDOW_X-128, upper_indent)));
	// Difficulty change
	menu.push_back(std::make_unique<MenuButton>(BACK, "Main Menu", font_size, sf::Vector2f(32, WINDOW_HEIGHT - 64 - font_size)));
	// "Back" button
	menu.push_back(std::make_unique<MenuButton>(PLAY, "Restart", font_size, sf::Vector2f(WINDOW_WIDTH- 256, WINDOW_HEIGHT - 64 - font_size)));

	for (size_t i = 0; i < menu.size(); i++) {
		intersects.push_back(false);
	}
}

void EndLevelMenu::update(GameState & game_state, sf::RenderWindow & window, const sf::Event & event)
{
	for (size_t i = 0; i < menu.size(); i++) {
		if (intersects[i] && event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
			switch (*(menu[i]->GetType()))
			{
			case PLAY: game_state = LEVEL1; break;
			case BACK: game_state = MAIN_MENU; break;
			}
		}
	}
}
