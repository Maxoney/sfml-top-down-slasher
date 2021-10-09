#pragma once
#include "include/Enemy.hpp"
#include "include/TextureStorage.hpp"


class Zombie : public Enemy {
public:
	Zombie(const sf::Texture& texture, Character* hero_, float* delta_);

	void update() override;
	void Movement();
	void Attack(float& dist);
private:
	bool within_reach = false; // дос€гаемость геро€
};