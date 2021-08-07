#include "include/Zombie.hpp"

Zombie::Zombie(const sf::Texture& texture, const std::string _name)
{
	sprite.setTexture(texture);	
	sprite.setTextureRect(sf::IntRect(0, 0, 37, 21));
	sprite.setOrigin(37 / 2, 21 / 2);
	float scale = 2;
	sprite.setScale({ scale,scale });
	sf::Vector2f pos = RandomCords();
	sprite.setPosition(pos);	// setting sprite spawn position
	x = pos.x;
	y = pos.y;
	fov = 300;
	speed = 1.5f;
	name = _name;
	hp = 4;
	dmg = 1;
	dmgResist = 1;
	cd_attack.SetTimer(1000);
}

void Zombie::update(float& delta, Character& hero)
{
	if (hp > 0) {
		state = IDLE;
		within_reach = false;
		sf::Vector2f heroCords = hero.GetCords();
		facing_vec = Normalize2(heroCords - sf::Vector2f(x, y));
		float dist = GetFastDistanceBP(heroCords, { x,y });
		if (dist < (fov*fov)) {		// если игрок в зоне видимости
			angle = GetAngleOfNormVec2(facing_vec);
			sprite.setRotation(90 + angle * 57.32);
			if (dist > 400) {
				state = WALKING;
				this->Movement();
			}
			this->Attack(hero, dist);
		}
	}
}

void Zombie::Movement()
{
	dx = facing_vec.x*speed;
	dy = facing_vec.y*speed;
	float incMov = (static_cast<float>(rand() % 11) / 10) - 0.5f;	// inconsistent movement- переменная непостоянного движения
	x += dx + incMov;	// delta добавляет подёргивания и неравномерную скорость движения (резкие)
	y += dy + incMov;	// без delta движение становится плавным и ровным (как по рельсам)
	sprite.setPosition(x, y);
}

void Zombie::Attack(Character & hero, float& dist)
{
	if (dist < 4001 && !attacking) {
		attacking = true;
//		std::cout << "Zombie attacks!\n";
		cd_attack.StartCooldown();
	}
	else if (attacking && cd_attack.IsEnded()) {
		attacking = false;
		if (dist > 4000) return;
		hero.RecieveDmg(dmg);
	}
}
