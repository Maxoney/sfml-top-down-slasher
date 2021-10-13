#include "..\include\Enemy.hpp"


Enemy::Enemy(Character * hero_, float * delta_)
	: hero(hero_), delta(delta_)
{
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

