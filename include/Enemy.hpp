#pragma once
#include "SFML/Graphics.hpp"
#include "include/Cooldown.hpp"
#include "include/quick_functions.hpp"
#include "include/Animation.hpp"


enum EnemyType {
	tZombie,
	tChimera
};

class Character;
//class Animation;

// родительский класс существ
class Enemy : public sf::Drawable {
public:
	// конструктор, который определяет спрайт и местоположение объекта (и название)
	Enemy(Character* hero_, const TextureStorage* texture, float* delta_);
	virtual ~Enemy() = default;

	virtual void update() = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void RecieveDamage(int dmg);
	float GetAngle() const { return angle; }
	const EnemyType& GetType() const { return type; }

	void setCords(sf::Vector2f cords) { position = cords; sprite.setPosition(position); }
	sf::Vector2f GetCords() const;
	sf::Vector2f GetFacingVec() const;
	CreatureState* GetState();

protected:
	const TextureStorage* txStorage;
	Character* hero;
	float* delta;
	Animation animation;
	sf::Sprite sprite;
	sf::Vector2f facing_vec;

	sf::Vector2f position;
	CreatureState state;
	Cooldown cd_attack;
	EnemyType type;
//	Animation anim_attack;

	float angle, fov, dist,	// координаты, угол смотрения и поле видимости, дистанция между врагом и игроком
		dx, dy, speed;			// смещение по х и у, а также коэффициент скорости
	float sprite_scale = 3;
	float knockback;	
	bool inFOV;			// игрок в поле видимости
	bool attacking;
	int hp, dmg, dmgResist;
};