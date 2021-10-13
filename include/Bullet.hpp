#pragma once
#include "SFML/Graphics.hpp"
#include "include/Cooldown.hpp"
#include "include/Character.hpp"
#include <cmath>
#include <queue>

enum BulletType {
	REGULAR,
	DEADLY,
	DAMAGING,
	DYING
};

class Bullet : public sf::Drawable{
public:
	Bullet(float _x, float _y, const sf::Vector2f& facing);
	~Bullet() = default;

	void update(Character& hero);
	void draw(sf::RenderTarget&, sf::RenderStates) const;

public:
	int dmg;

	BulletType type;

private:
	sf::Int32 lt;	// lifetime
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Clock clock;
	float x, y, fov,
		dx, dy, speed;

	sf::Vector2f facing_vec;
};