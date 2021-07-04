#pragma once
#include "SFML/Graphics.hpp"
#include "include/Cooldown.hpp"
#include "include/quick_functions.hpp"
#include "include/Character.hpp"

// родительский класс существ
class Enemy : public sf::Drawable {
public:
	// конструктор, который определяет спрайт и местоположение объекта (и название)
	Enemy();

	virtual void update(float& delta, Character& hero) = 0;
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void RecieveDamage(int dmg);
	float GetAngle() const;
	std::string GetName() const;

	sf::Vector2f GetCords() const;
	sf::Vector2f GetFacingVec() const;
	CreatureState* GetState();

protected:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f facing_vec;
	CreatureState state;
	Cooldown cd_attack;
//	Animation anim_attack;

	float x, y, angle, fov, dist,	// координаты, угол смотрения и поле видимости, дистанция между врагом и игроком
		dx, dy, speed;			// смещение по х и у, а также коэффициент скорости
	std::string name;
	bool inFOV;			// игрок в поле видимости
	bool attacking;
	int hp, dmg, dmgResist;
};