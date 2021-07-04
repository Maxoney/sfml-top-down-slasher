#include "..\include\Enemy.hpp"

Enemy::Enemy()
{
	texture.loadFromFile("res/sprites/missing.png");
	texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 128, 256));
	sprite.setOrigin(128 / 2, 256 / 2);
	sprite.setPosition(0, 0); // settin' sprite spawn position
	sprite.setScale(0.25, 0.25);
	x = 0.f;
	y = 0.f;
	fov = 0.f;
	inFOV = false;
	attacking = false;
	speed = 1.f;
	angle = 0.f;
	name = "missing enemy";
	state = IDLE;
	hp = 1;
	dmg = 0;
	dmgResist = 0;
}

void Enemy::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

sf::Vector2f Enemy::GetCords() const
{
	return sf::Vector2f(x, y);
}

sf::Vector2f Enemy::GetFacingVec() const
{
	return facing_vec;
}

float Enemy::GetAngle() const
{
	return angle;
}

std::string Enemy::GetName() const
{
	return name;
}

CreatureState * Enemy::GetState()
{
	return &state;
}

void Enemy::RecieveDamage(int dmg)
{
	if (dmgResist <= dmg) {
		hp -= dmg;
		if (hp < 1) {
			state = DEAD;
		}
	}
}

