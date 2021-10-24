#include "include/Zombie.hpp"

#include "include/Character.hpp"
#include "include/Weapon.hpp"
//#include "include/Animation.hpp"

Zombie::Zombie(const TextureStorage* texture, Character* hero_, float* delta_)
	:	Enemy(hero_, texture, delta_)
{
	sprite.setTexture(txStorage->GetTexture("Zombie"));
	sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	sprite.setOrigin((16 >> 1), 14);
	sprite.setScale(sprite_scale, sprite_scale);

	position = { 0 , 0 };
	sprite.setPosition(position); // settin' sprite spawn position

	fov = 300;
	speed = 1.5f;
	hp = 4;
	dmg = 1;
	dmgResist = 1;
	cd_attack.SetTimer(1000);
	type = EnemyType::tZombie;

	animation.Initialize(sprite, 0.2, 1, delta);

	axe = new Weapon(WeaponType::wtAXE, &position, &facing_vec, txStorage);
}

void Zombie::update()
{
	if (hp > 0) {
		state = IDLE;
		within_reach = false;
		facing_vec = Normalize2(hero->GetCords() - position);
		float dist = GetFastDistanceBP(hero->GetCords(), position);
		if (dist < (fov*fov)) {		// если игрок в зоне видимости
			angle = GetAngleOfNormVec2(facing_vec);
			//sprite.setRotation(90 + angle * 57.32);
			if (dist > 400.f) {
				this->Movement();
			}


			this->Attack(dist);

			if (facing_vec.x > 0) sprite.setScale(sprite_scale, sprite_scale);
			else sprite.setScale(-sprite_scale, sprite_scale);
			axe->update();
		}

		animation.update(static_cast<int>(state));
		sprite.setTextureRect(animation.GetBounds());

	}
}

void Zombie::Movement()
{
	state = WALKING;
	dx = facing_vec.x* (speed - knockback);
	dy = facing_vec.y* (speed - knockback);
	//knockback -= *delta * 6.f;	//	-0.1 while the game runs 60fps
	knockback = knockback < 0.1f ? 0.f : (knockback - *delta * 6.f);
	float incMov = (static_cast<float>(rand() % 11) / 10.f) - 0.5f;	// inconsistent movement- переменная непостоянного движения
	position.x += dx + incMov;	// delta добавляет подёргивания и неравномерную скорость движения (резкие)
	position.y += dy + incMov;	// без delta движение становится плавным и ровным (как по рельсам)
	sprite.setPosition(position);
}

void Zombie::Attack(float& dist)
{
	if (dist < 4001 && !attacking) {
		attacking = true;
//		std::cout << "Zombie attacks!\n";
		cd_attack.StartCooldown();
	}
	else if (attacking && cd_attack.IsEnded()) {
		attacking = false;
		if (dist > 4000) return;
		hero->RecieveDmg(dmg);
	}
}

void Zombie::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (facing_vec.x < 0) {
		if (cd_attack.IsEnded()) target.draw(*axe, states);
		target.draw(sprite, states);
	}
	else {
		target.draw(sprite, states);
		if (cd_attack.IsEnded()) target.draw(*axe, states);
	}
}
