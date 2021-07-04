#pragma once
#include "include/Cooldown.hpp"
#include "include/quick_functions.hpp"


class Powerup : public sf::Drawable {
public:
	Powerup(const sf::Vector2f& cords);
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	// should the powerup disappear?
	bool IsDead();

	sf::Vector2f GetCords();
	PowerupType GetType();

private:
	Cooldown lifetime;
	PowerupType type;
	sf::Texture texture;
	sf::Sprite sprite;

	float x, y;
};