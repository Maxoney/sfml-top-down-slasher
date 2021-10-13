#include "include/Chimera.hpp"
#include "include/Character.hpp"

Chimera::Chimera(const sf::Texture& texture, Character* hero_, float* delta_)
	: Enemy(hero_,delta_)
{
	sprite.setTexture(texture);
	sprite.setColor(sf::Color(255, 255, 255, 120));
	sprite.setTextureRect(sf::IntRect(0, 0, 28, 24));
	sprite.setOrigin(28 / 2, 24 / 2);
	float scale = 2;
	sprite.setScale({ scale,scale });
	x = 0;
	y = 0;
	sprite.setPosition(x, y);	// setting sprite spawn position
	fov = 600;
	hp = 2;
	dmg = 1;
	speed = 20;
	type = EnemyType::tChimera;
	cd_attack.SetTimer(700);
	cd_wait.SetTimer(500);
	cd_jump.SetTimer(700); //
}

void Chimera::update()
{
	if (hp > 0) {
		state = IDLE;
		facing_vec = Normalize2(hero->GetCords() - sf::Vector2f(x, y));
		float dist = GetFastDistanceBP(hero->GetCords(), { x,y });
		if (dist < (fov*fov)) {		// если игрок в зоне видимости
			angle = GetAngleOfNormVec2(facing_vec);
			sprite.setRotation(90 + angle * 57.32);
			state = WALKING;
			switch (doing) {

				case WAIT:
					if (cd_wait.IsEnded()) {
	//					this->Movement();
						doing = JUMP;
						if (dist > 10000) { // 100^2
							jump_angle = angle + (static_cast<float>(rand() % 29) / 10) - 1.4f; // кут у межах [x-40,x+40] градусів
						}
						else{
							jump_angle = angle + 3.15 + (static_cast<float>(rand() % 29) / 10) - 1.4f; // аналогічно, тільки в протилежну сторону
						}
						cd_jump.StartCooldown();
					}
					break;

				case JUMP:
					jump = 10 * EaseOutQuint(cd_jump.GetTime().asSeconds(), 0.4f);
					dx = cos(jump_angle)*speed;
					dy = sin(jump_angle)*speed;
					x += dx * jump;	
					y += dy * jump;	
					sprite.setPosition(x, y);
					if (jump < 0.05 && cd_jump.IsEnded()) {
						doing = ATTACK;
						state = ATTACKING;
						cd_attack.StartCooldown();
					}
					break;

				case ATTACK:
					if (cd_attack.IsEnded()) {
						doing = WAIT;
						cd_wait.StartCooldown();
					}
					break;

			}
		}
	}
}
