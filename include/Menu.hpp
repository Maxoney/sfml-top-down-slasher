#pragma once
#include "include/quick_functions.hpp"

static enum MenuButtonType {
	TEXT,
	IMAGE,
	PLAY,
	SETTINGS,
	HELP,
	EXIT,
	PREVIOUS,
	NEXT,
	BACK,
	SETTINGS_CONTROLS,
	SETTINGS_DIFFICULTY
};



class MenuButton : public sf::Drawable {	// второй ненавидимый мною класс
public:
	MenuButton(MenuButtonType _type, const std::string str, const int& size, const sf::Vector2f& cords);

	bool Fading(const sf::Vector2f& mouse_pos);
	void draw(sf::RenderTarget&, sf::RenderStates) const;

	MenuButtonType* GetType();
	sf::Sprite* GetSprite();
	sf::Texture* GetTexture();
	sf::Text* ChangeText();
	sf::RectangleShape* GetBoundingBox();

private:
	MenuButtonType type;
	sf::Font font;
	sf::RectangleShape boundingBox;
	sf::Text text;
	sf::Vector2f cords;
	sf::Texture texture;
	sf::Sprite sprite;

	int page = 0;
};



class Menu : public sf::Drawable {
public:
	Menu() = default;

	// animation in "update" method does't work as intended because of "clicking handling"
	void Animation(const sf::Vector2f & mouse_pos);
	virtual void update(sf::RenderWindow& window, const sf::Event& event) = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const;

protected:
	std::vector<bool> intersects;
	std::vector<std::unique_ptr<MenuButton>> menu;
};



class MainMenu : public Menu {
public:
	MainMenu(const int& font_size);

	void update(sf::RenderWindow& window, const sf::Event& event);
};



class SettingsMenu : public Menu {
public:
	SettingsMenu(const int& font_size);

	void update(sf::RenderWindow& window, const sf::Event& event);

private:
	GameDificulty* difficulty;
	ControlsType* controls;
};



class HelpMenu : public Menu {
public:
	HelpMenu(const int& font_size);

	void update(sf::RenderWindow& window, const sf::Event& event);

private:
	int menu_page = 0;
	std::vector<sf::Sprite*> p_sprites;
	std::vector<sf::Texture*> p_textures;
};



class EndLevelMenu : public Menu {
public:
	EndLevelMenu(const int font_size);

	void update(sf::RenderWindow& window, const sf::Event& event);
};