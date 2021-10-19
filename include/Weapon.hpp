#pragma once
#include "include/quick_functions.hpp"

class Cooldown;

enum WeaponType {
	wtSWORD,
	wtHAMMER,
	wtAXE,
	wtSTAFF
};

class Weapon : public sf::Drawable {
public:
	Weapon(WeaponType type, const sf::Vector2f* pos, const sf::Vector2f* fac_vec, const TextureStorage* storage);
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void update();

private:
	const TextureStorage* txStorage;
	const sf::Vector2f* position;
	const sf::Vector2f* facing_vec;
	//sf::RectangleShape shape;
	sf::Sprite sprite;
	float sprite_scale = 2;
	float angle;
};