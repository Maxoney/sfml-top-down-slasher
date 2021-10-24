#pragma once
#include "include/Animation.hpp"
//#include <iostream>
#include "include/quick_functions.hpp"

class Blade : public sf::Drawable {
public:
	Blade(const std::string dir, float scale, const float* delta_);

	void draw(sf::RenderTarget&, sf::RenderStates) const;	// на данный момент чисто отладочный метод
//	void attack(sf::Event event);
//	void block(sf::Event event);
	void update(sf::Vector2f cords, float angle, float delta);

	Animation* GetAnimation();
private:
	Animation animation;
	sf::Texture texture;
	sf::Sprite sprite;
};