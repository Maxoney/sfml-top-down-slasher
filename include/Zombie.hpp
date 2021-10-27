#pragma once
#include "include/Enemy.hpp"
#include "include/TextureStorage.hpp"

class Weapon;

class Zombie : public Enemy {
public:
	Zombie(const TextureStorage* texture, Character* hero_, float* delta_);
	~Zombie();

	void update() override;
	void Movement();
	void Attack(float& dist);
	void draw(sf::RenderTarget&, sf::RenderStates) const override;
private:
	Weapon* axe;
	bool within_reach = false; // ������������ �����
};