#pragma once
#include <SFML/Graphics.hpp>
#include "include/Cooldown.hpp"
//#include <iostream>
#include <stdint.h>
#include "include/Blade.hpp"
#include "include/quick_functions.hpp"
#include "include/globalVar.hpp"
#include "include/Powerup.hpp"

class Enemy;
class Weapon;

class Character : public sf::Drawable {
public:
	// конструктор, который определяет спрайт и местоположение объекта (а также имя)
	Character(const TextureStorage* texture, float _x, float _y, const sf::RenderWindow* window_, const float* delta_);
	~Character();
	// attack method
	void Attack();
	// finding out moving direction
	void Controls(const sf::Event* event);
	void Movement();
	// starting charge
	void Charge();
	void ChangeControlsType() { controls_fixed^=true; };
	// detecting some collision
	void CollisionDetection();
	void update();
	void draw(sf::RenderTarget&, sf::RenderStates) const;
	void updateControls();
	void updateDifficulty();
	// set the vector of enemies
	void SetEnemies(vec_uptrEnemy* enemies_)	{	enemies = enemies_;	}
	void SetCollisionMap(std::vector<bool>* map) { collision_map = map; }
	void SetPosition(const float& _x, const float& _y);
	// handling powerup pickup
	void PickUpPowerup(const PowerupType& type);
	// handling stamina loose from external factors (currently not used)
	void StaminaDrain(int& num);
	void RecieveDmg(int& dmg);
	void Reset();

	float GetAngle() const;
	int GetDmg() const;


	const sf::Vector2f& GetCords() const;
	sf::Vector2f GetFacingVec() const;
	const sf::Vector2i* GetHP() const;
	const sf::Vector2i* GetSP() const;
	CreatureState GetState() const;
	const Cooldown* GetAttackCd() const { return &cd_attack; }
	const Cooldown* GetChargeCd() const { return &cd_charge; }
	const Cooldown* GetPowerUpCd() const { return &cd_powerup; }
	const PowerupType* GetPowerupType() const { return &powerup; }

private:
	void TerrainCollision(bool left, bool up);

public:
	CreatureState state = IDLE;
	Cooldown cd_charge, cd_attack, cd_powerup,
						cd_attack_anim;
	PowerupType powerup;

	bool attacking = false;
	bool charging = false;

private:
	Weapon* sword;
	const TextureStorage* txStorage;
	vec_uptrEnemy* enemies;
	std::vector<bool>* collision_map;
	const sf::RenderWindow* window;
	const float* delta;
	Blade blade;
	sf::Sprite sprite;
	std::string name;
	sf::Vector2i hp, sp;	//	{max_Xp, current_Xp}
	sf::Vector2f facing_vec;
	sf::Vector2f position;
	sf::RectangleShape coll_rect;	//	collision rectangle
	float angle; // угол смотрения персонажа
	float dx, dy;
	float sprite_scale = 3;
	float sqrt2 = 1, charge = 1, speed = 100;// допоміжні параметры для передвижения персонажа
	int dmg; 
	bool controls_fixed = false; // выключатель управления "типа от первого лица"
	bool dirup = 0, // direction
		dirdown = 0,
		dirleft = 0,
		dirright = 0;

};