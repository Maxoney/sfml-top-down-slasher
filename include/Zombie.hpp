#pragma once
#include "include/Enemy.hpp"


class Zombie : public Enemy {
public:
	Zombie(const std::string dir, const std::string _name);

	void update(float& delta, Character& hero) override;
	void Movement();
	void Attack(Character& hero, float& dist);
private:
	bool within_reach = false; // дос€гаемость геро€
};