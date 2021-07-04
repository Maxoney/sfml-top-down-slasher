#pragma once
#include "SFML/Graphics.hpp"
#include "include/quick_functions.hpp"
#include "include/Character.hpp"

enum UIComponentType {
	BAR_HP,
	BAR_SP,
	BAR_ENEMY_COUNTER,
	ICON_ATTACK,
	ICON_CHARGE,
	ICON_POWERUP
};



class UIComponent : public sf::Drawable {	// плохой класс, который я не хотел разделять на родительский и дочерние
public:
	UIComponent(const std::string dir, UIComponentType _type,int _sx, int _sy, int height, int width, float scale);

	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void update(const sf::Vector2f& cords, const Character& hero);

private:
	bool first_time = true;
	float x, y, scale;
	int sx, sy, height, width;

	UIComponentType type;
	sf::Font uifont;
	sf::Text text;
	sf::Texture texture;
	sf::RectangleShape rect;
	sf::Sprite sprite;
};