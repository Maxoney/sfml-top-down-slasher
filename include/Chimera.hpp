#pragma once
#include "include/Enemy.hpp"
#include "include/Bullet.hpp"
#include <list>

enum ChimDoes {
	JUMP,
	ATTACK,
	WAIT
};

// Looks more like gargoyle than chimera
class Chimera : public Enemy {
public:
	Chimera(const std::string dir, const std::string _name);

	void update(float& delta, Character& hero) override;
	float GetAngle() const;

	sf::Vector2f GetCords() const;

private:
	float jump = 0, jump_angle;
	bool waiting = true;
	int dmg = 3;

	ChimDoes doing = WAIT;
	Cooldown cd_jump, cd_wait, cd_attack;
};