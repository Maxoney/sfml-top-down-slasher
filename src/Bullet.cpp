#include "include/Blade.hpp"
#include "include/Bullet.hpp"

Bullet::Bullet(float _x, float _y, const sf::Vector2f& facing)
{
	texture.loadFromFile("res/sprites/bullet.png");
	texture.setSmooth(false);
	sprite.setTexture(texture);
	if (rand() % 5 == 1) {
		type = DEADLY;
		sprite.setTextureRect(sf::IntRect(4, 0, 4, 6));
		speed = 3.5;
		dmg = 10;
	}
	else {
		type = REGULAR;
		sprite.setTextureRect(sf::IntRect(0, 0, 4, 6));
		speed = 4;
		dmg = 3;
	}
	sprite.setOrigin(2, 2);
	sprite.setPosition(_x, _y); // settin' sprite spawn position
	sprite.setScale(2, 2);
	x = _x;
	y = _y;
	facing_vec = facing;
	sprite.setRotation(90 + GetAngleOfNormVec2(facing_vec) * 57.32);
	lt = 10;
	clock.restart();
}

void Bullet::update(Character& hero)
{
	if (clock.getElapsedTime().asSeconds() > lt) type = DYING;
	sf::Vector2f heroCords = hero.GetCords();
	dx = facing_vec.x*speed;
	dy = facing_vec.y*speed;
	x += dx;
	y += dy;
	sprite.setPosition(x, y);	
	if (GetFastDistanceBP(heroCords, { x,y }) < 1600) {	// 40^2 - у€вний х≥тбокс гравц€ в квадрат≥
		type = DAMAGING;
	}
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

