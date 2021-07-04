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

class Bullet {
public:
	Bullet(float _x, float _y, const sf::Vector2f& facing);
	~Bullet() = default;

	void update(Character& hero);

public:
	int dmg;

	BulletType type;
	sf::Sprite sprite;

private:
	float x, y, fov,
		dx, dy, speed;

	sf::Vector2f facing_vec;
	sf::Texture texture;
	sf::Clock clock;
	sf::Int32 lt;	// lifetime
};