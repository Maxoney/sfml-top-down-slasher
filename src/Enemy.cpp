#include "..\include\Enemy.hpp"

Enemy::Enemy()
{
	std::cerr << "Empty enemy\n";
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

